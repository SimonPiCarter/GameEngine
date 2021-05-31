#pragma once

#include <array>

class LogicEngine;
struct WaveLayout;
struct MobModel;

class WaveEngine
{
public:
	WaveEngine(LogicEngine &logic_p);

	void waveLoop(WaveLayout const &layout_p);

	void spawnMob(MobModel const &model_p, std::array<long, 2> const & spawnPosition_p);
protected:
	LogicEngine &_logic;

	bool isWaveOver();
};
