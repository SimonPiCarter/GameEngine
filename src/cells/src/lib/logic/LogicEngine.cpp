#include "LogicEngine.h"

#include "WaveEngine.h"

LogicEngine::LogicEngine()
{}

void LogicEngine::run()
{
	WaveEngine wave_l(*this);

	//wave_l.waveLoop();
}
