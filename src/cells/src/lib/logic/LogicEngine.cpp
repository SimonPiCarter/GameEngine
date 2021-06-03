#include "LogicEngine.h"

#include "WaveEngine.h"
#include "logic/entity/Tower.h"

LogicEngine::LogicEngine(MapLayout const * map_p)
	: _quit(false)
	, _currentMap(map_p)
{}

LogicEngine::~LogicEngine()
{
	for(Tower * tower_l : _towers)
	{
		delete tower_l;
	}
}


void LogicEngine::run()
{
	WaveEngine wave_l(*this);

	//wave_l.waveLoop();
}
