#include "LogicEngine.h"

#include "game/CellsEngine.h"
#include "logic/WaveEngine.h"
#include "logic/display/MapDisplay.h"
#include "logic/entity/Tower.h"

LogicEngine::LogicEngine(MapLayout const * map_p, CellsEngine * cellsEngine_p)
	: _cellsEngine(cellsEngine_p)
	, _quit(false)
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
	}
	if(_waveEngine)
	{
		_waveEngine->handleFrame(elapsedTime_p);
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
}

void LogicEngine::spawnMob(MobEntity * entity_p, std::array<double, 2> const & spawnPosition_p)
{
	if(_cellsEngine)
	{
		GraphicEntity * entity_l = new GraphicEntity();
		_cellsEngine->getGraphic().registerMessage(new NewGraphicEntityMessage(entity_l, entity_p->getModel()->resource,
			{spawnPosition_p[0]-entity_p->getSize()[0]/2., spawnPosition_p[1]-entity_p->getSize()[1]/2., 0.}, "game"));
		entity_p->setGraphic(entity_l);
	}
}

void LogicEngine::despawnMob(MobEntity * entity_p)
{
	if(_cellsEngine && entity_p->getGraphic())
	{
		_cellsEngine->getGraphic().registerMessage(new DestroyGraphicEntityMessage(entity_p->getGraphic()));
	}
}

void LogicEngine::killMob(MobEntity * entity_p)
{
	if(_cellsEngine && entity_p->getGraphic())
	{
		_cellsEngine->getGraphic().registerMessage(new DestroyGraphicEntityMessage(entity_p->getGraphic()));
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

void LogicEngine::spawnTower(Tower * tower_p)
{
	_towers.push_back(tower_p);

	if(_cellsEngine && !tower_p->getResource().empty())
	{
		GraphicEntity * entity_l = new GraphicEntity();
		_cellsEngine->getGraphic().registerMessage(new NewGraphicEntityMessage(entity_l, tower_p->getResource(),
			{tower_p->getPosition()[0]-tower_p->getSize()[0]/2., tower_p->getPosition()[1]-tower_p->getSize()[1]/2., 0.}, "game"));
		tower_p->setGraphic(entity_l);
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
