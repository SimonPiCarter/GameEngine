#pragma once

#include <array>

#include "wave/PositionalTreeDefs.h"
#include "entity/MobEntity.h"

class LogicEngine;
struct WaveLayout;
struct MobModel;
class MobSpawner;

/// @brief is responsible for a wave
class WaveEngine
{
public:
	WaveEngine(LogicEngine &logic_p);
	~WaveEngine();

	void waveLoop(WaveLayout const &layout_p);

	void handleFrame(double elapsedTime_p);

	void spawnMob(MobModel const &model_p, std::array<double, 2> const & spawnPosition_p);

	PositionalTree<MobEntity> const &getTree() const { return _tree; }
	PositionalTree<MobEntity> &getTree() { return _tree; }
protected:
	LogicEngine &_logic;

	bool isWaveOver();

	/// @brief handle massive entity count positions
	PositionalTree<MobEntity> _tree;

	std::list<MobEntity *> _mobs;

	MobSpawner * _spawner;
	bool _spawnOver;
};
