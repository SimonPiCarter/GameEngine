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
}


void LogicEngine::init()
{
	delete _waveEngine;
	_waveEngine = new WaveEngine(*this);
	if(_cellsEngine)
	{
		MapDisplay * display_l = newMapDisplay("root", _currentMap, _cellsEngine->getGraphic());

		GraphicEntity light_l;
		_cellsEngine->getGraphic().registerMessage(new NewLightMessage(&light_l, "root", {1., 1., 1.}, {-1, -1, -1}, LightType::Directional));

		_cellsEngine->getGraphic().registerMessage(new MoveCameraMessage({10., 3.5, 10.}));
		_cellsEngine->getGraphic().registerMessage(new LookAtCameraMessage({4., 3.5, 0.}));
		_cellsEngine->getGraphic().registerMessage(new RotateCameraMessage({0., 0., 90.}));

		//_waveEngine->init(??);
	}
}

void LogicEngine::run(double elapsedTime_p)
{
	if(_waveEngine)
	{
		_waveEngine->handleFrame(elapsedTime_p);
	}
}
