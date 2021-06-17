#pragma once

#include <random>

class WaveLayout;

/// @brief generate waves based on a random seed
/// abstract class to allow override in generation
/// keep track of last generated wave in memory for
/// memory consistancy (delete it when next wave is generated)
class WaveGenerator
{
public:
	WaveGenerator(unsigned long seed_p);
	virtual ~WaveGenerator();

	WaveLayout * getNextWave();
protected:
	std::mt19937_64 _generator;

	virtual WaveLayout * nextWave() = 0;

	WaveLayout * _lastWave;
};
