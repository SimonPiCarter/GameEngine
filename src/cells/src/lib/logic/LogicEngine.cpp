#include "LogicEngine.h"

#include "game/CellsEngine.h"
#include "logic/WaveEngine.h"
#include "logic/display/MapDisplay.h"
#include "logic/entity/Tower.h"
#include "logic/slot/Slot.h"
#include "logic/ui/HeaderUI.h"
#include "logic/ui/MobSelectionUI.h"
#include "logic/ui/InventoryUI.h"
#include "logic/ui/TowerSelectionUI.h"
#include "logic/generator/wave/WaveGenerator.h"
#include "logic/Constant.h"


LogicEngine::LogicEngine(MapLayout const * map_p, CellsEngine * cellsEngine_p)
	: _cellsEngine(cellsEngine_p)
	, _ui()
	, _inventoyHidden(true)
	, _tree({{0.,0.}, {double(map_p->getSize()[0]), double(map_p->getSize()[1])}}, map_p->getSize()[0], 0.)
	, _quit(false)
	, _life(100.)
	, _scrap(100.)
	, _time(0.)
	, _mobSelection(nullptr)
	, _towerSelection(nullptr)
	, _currentMap(map_p)
	, _nextWave(nullptr)
	, _waveEngine(nullptr)
	, _isWaveRunning(false)
	, _placingTower(false)
	, _isOver(false)
	, _timeToWave(0.)
{}

LogicEngine::~LogicEngine()
{
	for(Tower * tower_l : _towers)
	{
		delete tower_l;
	}
	for(GraphicEntity * light_l : _lights)
	{
		delete light_l;
	}

	for(std::pair<GraphicEntity *, double> const &pair_l : _particles)
	{
		delete pair_l.first;
	}
}


void LogicEngine::init()
{
	if(_cellsEngine)
	{
		MapDisplay * display_l = newMapDisplay("root", _currentMap, _cellsEngine->getGraphic());

		_lights.push_back(new GraphicEntity());
		_cellsEngine->getGraphic().registerMessage(new NewLightMessage(_lights.back(), "root", {2.5, 6.5, 0.1}, {-1, -1, -2}, LightType::Point, 5., false
			, {1., 0.8, 0.8}, {1., 0.8, 0.8}, {0., 0.}));
		_lights.push_back(new GraphicEntity());
		_cellsEngine->getGraphic().registerMessage(new NewLightMessage(_lights.back(), "root", {2.5, 5.5, 1.5}, {-1, -1, -2}, LightType::Directional, 0.5, true));

		double size_l = std::max<double>(_currentMap->getSize()[0], _currentMap->getSize()[1]);
		_cellsEngine->getGraphic().registerMessage(new MoveCameraMessage({size_l*1.2, size_l/2. - 0.5, size_l}));
		_cellsEngine->getGraphic().registerMessage(new LookAtCameraMessage({size_l/2., size_l/2. - 0.5, 0.}));
		_cellsEngine->getGraphic().registerMessage(new RotateCameraMessage({0., 0., 90.}));

		_cellsEngine->getGraphic().registerMessage(new NewSceneMessage("game", "root", {0.,0.,0.}));

		_ui.init(*this);
	}
}

void LogicEngine::run(double elapsedTime_p)
{
	if(!_nextWave)
	{
		_nextWave = _cellsEngine->getWaveGenerator()->getNextWave();
		if(_nextWave)
		{
			_timeToWave = _nextWave->time;
		}
	}
	if(!_nextWave)
	{
		_isOver = true;
	}
	bool startWave_l = false;

	// update start wave if necessary
	if(elapsedTime_p > _timeToWave && !_isOver)
	{
		startWave_l = true;
	}
	else
	{
		_timeToWave -= elapsedTime_p;
	}

	// start wave
	if(!_waveEngine && startWave_l)
	{
		_waveEngine = new WaveEngine(*this);
		_waveEngine->init(*_nextWave);
		_isWaveRunning = true;
		startWave_l = false;
		_time = 0.;

		// close inventory and leave place tower mode
		setInventoryHidden(true);
		setPlacingTower(false);
	}

	// run wave
	if(_waveEngine)
	{
		_waveEngine->handleFrame(elapsedTime_p);
		_time += elapsedTime_p;
		// end wave
		if(_waveEngine->isWaveOver())
		{
			addSlotsToInventory(_nextWave->rewards);
			delete _waveEngine;
			_waveEngine = nullptr;
			_isWaveRunning = false;
			_nextWave = nullptr;
		}
	}

	// particle handling
	for(auto it_l = _particles.begin() ; it_l != _particles.end() ; )
	{
		// update lifetime of particle
		it_l->second = it_l->second - elapsedTime_p;
		if(it_l->second <= 0.)
		{
			_cellsEngine->getGraphic().registerMessage(new DestroyParticleMessage(it_l->first));
			it_l = _particles.erase(it_l);
		}
		else
		{
			++ it_l;
		}
	}
	_ui.update();
}

void LogicEngine::spawnMob(MobEntity * entity_p, std::array<double, 2> const & spawnPosition_p)
{
	if(_cellsEngine)
	{
		GraphicEntity * entity_l = new GraphicEntity();
		_cellsEngine->getGraphic().registerMessage(new NewGraphicEntityMessage(entity_l, entity_p->getModel()->resource,
			{spawnPosition_p[0]-entity_p->getSize()[0]/2., spawnPosition_p[1]-entity_p->getSize()[1]/2., 0.}, "game"));
		entity_p->setGraphic(entity_l);
		entity_l->addData("mob", entity_p);
	}
}

void LogicEngine::despawnMob(MobEntity * entity_p)
{
	if(_cellsEngine && entity_p->getGraphic())
	{
		_cellsEngine->getGraphic().registerMessage(new DestroyGraphicEntityMessage(entity_p->getGraphic(), true));
	}

	// decrease life
	_life -= entity_p->getModel()->life_dmg;

	// reset selection
	if(_mobSelection == entity_p)
	{
		_mobSelection = nullptr;
	}
}

void LogicEngine::killMob(MobEntity * entity_p)
{
	if(_cellsEngine && entity_p->getGraphic())
	{
		_cellsEngine->getGraphic().registerMessage(new DestroyGraphicEntityMessage(entity_p->getGraphic(), true));
	}

	// increase scrap
	_scrap += entity_p->getModel()->scrap_reward;

	// reset selection
	if(_mobSelection == entity_p)
	{
		_mobSelection = nullptr;
	}
}

void LogicEngine::moveMob(MobEntity * entity_p, std::array<double,2> oldPos_p, std::array<double,2> pos_p)
{
	if(_cellsEngine)
	{
		_cellsEngine->getGraphic().registerMessage(new MoveGraphicEntityMessage(entity_p->getGraphic(),
			{pos_p[0] - oldPos_p[0], pos_p[1] - oldPos_p[1], 0.}));
	}
}

void LogicEngine::spawnDamageParticle(std::array<double,2> pos_p, double lifetime_p)
{
	if(_cellsEngine)
	{
		_particles.push_back(std::pair<GraphicEntity*, double>(new GraphicEntity(), lifetime_p));
		_cellsEngine->getGraphic().registerMessage(new NewParticleMessage(_particles.back().first, "Particle/impact", {pos_p[0],pos_p[1],0.}, "game"));
		_particles.push_back(std::pair<GraphicEntity*, double>(new GraphicEntity(), lifetime_p));
		_cellsEngine->getGraphic().registerMessage(new NewParticleMessage(_particles.back().first, "Particle/impact_point", {pos_p[0],pos_p[1],0.}, "game"));
	}
}

bool LogicEngine::spawnTower(Tower * tower_p)
{
	if(_scrap >= TOWER_COST)
	{
		_towers.push_back(tower_p);
		_towers.back()->setMainHitbox({{0.,0., 0.}, {tower_p->getSize()[0], tower_p->getSize()[1], 1.}});
		_tree.addContent(tower_p);

		if(_cellsEngine && !tower_p->getResource().empty())
		{
			GraphicEntity * entity_l = new GraphicEntity();
			entity_l->addData("tower", tower_p);
			_cellsEngine->getGraphic().registerMessage(new NewGraphicEntityMessage(entity_l, tower_p->getResource(),
				{tower_p->getPosition()[0]-tower_p->getSize()[0]/2., tower_p->getPosition()[1]-tower_p->getSize()[1]/2., 0.}, "game"));
			tower_p->setGraphic(entity_l);
		}
		_scrap -= TOWER_COST;
		return true;
	}
	delete tower_p;
	return false;
}

void LogicEngine::deleteSelectedTower()
{
	if(!_towerSelection || _isWaveRunning)
	{
		return;
	}
	if(_cellsEngine && _towerSelection->getGraphic())
	{
		_cellsEngine->getGraphic().registerMessage(new DestroyGraphicEntityMessage(_towerSelection->getGraphic(), true));
	}

	// increase scrap
	_scrap += TOWER_COST/2.;

	// delete all slots of the tower
	std::set<Slot *> slots_l;
	slots_l.insert(new AttackModifier(_towerSelection->getAttackModifier()));
	for(Slot * slot_l : _towerSelection->getSlots())
	{
		if(slot_l)
		{
			slots_l.insert(slot_l);
		}
	}
	deleteSlots(slots_l);

	// reset selection
	setTowerSelection(nullptr);
}

MobEntity * LogicEngine::getMobSelection(std::array<double, 3> pos_p, std::array<double, 3> dir_p)
{
	if(!_waveEngine)
	{
		return nullptr;
	}
	return _waveEngine->getTree().getIntersectionToRay(pos_p, dir_p);
}

Tower * LogicEngine::getTowerSelection(std::array<double, 3> pos_p, std::array<double, 3> dir_p)
{
	return _tree.getIntersectionToRay(pos_p, dir_p);
}

void LogicEngine::setInventoryHidden(bool hidden_p)
{
	if(_ui._inventoryUI)
	{
		if(hidden_p)
		{
			_cellsEngine->getGraphic().registerMessage(new CustomGuiMessage(hide_inventory, _ui._inventoryUI));
		}
		else
		{
			_cellsEngine->getGraphic().registerMessage(new CustomGuiMessage(show_inventory, _ui._inventoryUI));
		}
	}
	_inventoyHidden = hidden_p;
}

bool LogicEngine::isInventoryHidden()
{
	return _inventoyHidden;
}

void LogicEngine::addSlotsToInventory(std::list<Slot *> const &newSlots_p)
{
	auto it_l = newSlots_p.begin();
	for(size_t i = 0 ; i < _inventorySlots.size() && it_l != newSlots_p.end() ; ++i)
	{
		if(_inventorySlots[i] == nullptr)
		{
			_inventorySlots[i] = *it_l;
			++ it_l;
		}
	}
	_inventorySlots.insert(_inventorySlots.end(), it_l, newSlots_p.end());
}

void LogicEngine::updateInventory(std::set<Slot *> const &consumedSlots_p)
{
	std::vector<Slot *> newInventory_l;
	for(Slot * slot_l : _inventorySlots)
	{
		if(consumedSlots_p.find(slot_l) == consumedSlots_p.end())
		{
			newInventory_l.push_back(slot_l);
		}
	}
	std::swap(newInventory_l, _inventorySlots);
	if(_inventorySlots.size() < MIN_INVENTORY_SIZE)
	{
		_inventorySlots.resize(MIN_INVENTORY_SIZE,nullptr);
	}
}

void LogicEngine::deleteSlots(std::set<Slot *> const &toBeRemovedSlots_p)
{
	for(Slot * slot_l : toBeRemovedSlots_p)
	{
		_scrap += slot_l->getLvl();
		delete slot_l;
	}
}

void LogicEngine::updateTowerSelection()
{
	_ui._towerSelectionUI->update(true);
}
