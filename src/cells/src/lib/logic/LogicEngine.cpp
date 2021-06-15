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

LogicEngine::LogicEngine(MapLayout const * map_p, CellsEngine * cellsEngine_p)
	: _cellsEngine(cellsEngine_p)
	, _header(nullptr)
	, _mobSelectionUI(nullptr)
	, _towerSelectionUI(nullptr)
	, _inventoryUI(nullptr)
	, _inventoyHidden(true)
	, _tree({{0.,0.}, {double(map_p->getSize()[0]), double(map_p->getSize()[1])}}, map_p->getSize()[0], 0.)
	, _quit(false)
	, _life(100.)
	, _scrap(20.)
	, _time(0.)
	, _mobSelection(nullptr)
	, _towerSelection(nullptr)
	, _currentMap(map_p)
	, _waveEngine(nullptr)
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
	delete _header;
	delete _mobSelectionUI;
	delete _towerSelectionUI;
	delete _inventoryUI;
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

		_header = new HeaderUI(*this);
		_mobSelectionUI = new MobSelectionUI(*this);
		_towerSelectionUI = new TowerSelectionUI(*this);
		_inventoryUI = new InventoryUI(*this);
	}
}

void LogicEngine::run(double elapsedTime_p)
{
	bool startWave_l = true;
	WaveLayout wave_l = _cellsEngine->getNextWave();

	if(!_waveEngine && startWave_l)
	{
		_waveEngine = new WaveEngine(*this);
		_waveEngine->init(wave_l);
		startWave_l = false;
		_time = 0.;
	}
	if(_waveEngine)
	{
		_waveEngine->handleFrame(elapsedTime_p);
		_time += elapsedTime_p;
		if(_waveEngine->isWaveOver())
		{
			delete _waveEngine;
			_waveEngine = nullptr;
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

	if(_header)
	{
		_header->update();
	}
	if(_mobSelectionUI)
	{
		_mobSelectionUI->update();
	}
	if(_towerSelectionUI)
	{
		_towerSelectionUI->update();
	}
	if(_inventoryUI)
	{
		_inventoryUI->update();
	}
}

void LogicEngine::spawnMob(MobEntity * entity_p, std::array<double, 2> const & spawnPosition_p)
{
	if(_cellsEngine)
	{
		GraphicEntity * entity_l = new GraphicEntity();
		_cellsEngine->getGraphic().registerMessage(new NewGraphicEntityMessage(entity_l, entity_p->getModel()->resource,
			{spawnPosition_p[0]-entity_p->getSize()[0]/2., spawnPosition_p[1]-entity_p->getSize()[1]/2., 0.}, "game"));
		entity_p->setGraphic(entity_l);
		entity_l->getData().setUserAny("mob", Ogre::Any(entity_p));
	}
}

void LogicEngine::despawnMob(MobEntity * entity_p)
{
	if(_cellsEngine && entity_p->getGraphic())
	{
		_cellsEngine->getGraphic().registerMessage(new DestroyGraphicEntityMessage(entity_p->getGraphic()));
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
		_cellsEngine->getGraphic().registerMessage(new DestroyGraphicEntityMessage(entity_p->getGraphic()));
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

void LogicEngine::spawnTower(Tower * tower_p)
{
	_towers.push_back(tower_p);
	_towers.back()->setMainHitbox({{0.,0., 0.}, {tower_p->getSize()[0], tower_p->getSize()[1], 1.}});
	_tree.addContent(tower_p);

	if(_cellsEngine && !tower_p->getResource().empty())
	{
		GraphicEntity * entity_l = new GraphicEntity();
		entity_l->getData().setUserAny("tower", Ogre::Any(tower_p));
		_cellsEngine->getGraphic().registerMessage(new NewGraphicEntityMessage(entity_l, tower_p->getResource(),
			{tower_p->getPosition()[0]-tower_p->getSize()[0]/2., tower_p->getPosition()[1]-tower_p->getSize()[1]/2., 0.}, "game"));
		tower_p->setGraphic(entity_l);
	}
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
	if(_inventoryUI)
	{
		if(hidden_p)
		{
			_cellsEngine->getGraphic().registerMessage(new CustomGuiMessage(hide_inventory, _inventoryUI));
		}
		else
		{
			_cellsEngine->getGraphic().registerMessage(new CustomGuiMessage(show_inventory, _inventoryUI));
		}
	}
	_inventoyHidden = hidden_p;
}

bool LogicEngine::isInventoryHidden()
{
	return _inventoyHidden;
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
}

void LogicEngine::deleteSlots(std::set<Slot *> const &toBeRemovedSlots_p)
{
	for(Slot * slot_l : toBeRemovedSlots_p)
	{
		_scrap += slot_l->getLvl();
		delete slot_l;
	}
}

