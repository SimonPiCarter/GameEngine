#include "LogicEngine.h"

#include "game/CellsEngine.h"
#include "logic/WaveEngine.h"
#include "logic/display/MapDisplay.h"
#include "logic/entity/Tower.h"
#include "logic/layout/WaveLayout.h"

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
	if(_cellsEngine)
	{
		MapDisplay * display_l = newMapDisplay("root", _currentMap, _cellsEngine->getGraphic());

		GraphicEntity light_l;
		_cellsEngine->getGraphic().registerMessage(new NewLightMessage(&light_l, "root", {1., 1., 1.}, {-1, -1, -1}, LightType::Directional));

		double size_l = std::max<double>(_currentMap->getSize()[0], _currentMap->getSize()[1]);
		_cellsEngine->getGraphic().registerMessage(new MoveCameraMessage({size_l*1.2, size_l/2. - 0.5, size_l}));
		_cellsEngine->getGraphic().registerMessage(new LookAtCameraMessage({size_l/2., size_l/2. - 0.5, 0.}));
		_cellsEngine->getGraphic().registerMessage(new RotateCameraMessage({0., 0., 90.}));
	}
}

WaveLayout getNextWave()
{
	WaveLayout layout_l;
	layout_l.mobLayout.push_back(
		{
			{12., 1., ArmorType::Standard, "Cube", {0.1,0.1}},		// Mob model
			10,														// number of spawn
			1.														// interval
		}
	);
	return layout_l;
}

void LogicEngine::run(double elapsedTime_p)
{
	bool startWave_l = true;
	WaveLayout wave_l = getNextWave();

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
}
