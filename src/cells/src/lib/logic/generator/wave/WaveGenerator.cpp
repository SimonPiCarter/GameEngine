#include "WaveGenerator.h"

#include "logic/layout/WaveLayout.h"

WaveGenerator::WaveGenerator(unsigned long seed_p)
	: _generator(seed_p)
	, _lastWave(nullptr)
{}

WaveGenerator::~WaveGenerator()
{}

WaveLayout * WaveGenerator::getNextWave()
{
	delete _lastWave;
	_lastWave = nextWave();
	return _lastWave;
}
