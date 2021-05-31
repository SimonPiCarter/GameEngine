#include "LogicEngine.h"

#include "WaveEngine.h"

LogicEngine::LogicEngine()
	: _quit(false)
	, _currentMap(nullptr)
{}

void LogicEngine::run()
{
	WaveEngine wave_l(*this);

	//wave_l.waveLoop();
}
