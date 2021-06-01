#pragma once

#include <array>

#include "wave/PositionalTreeDefs.h"
#include "entity/MobEntity.h"

class LogicEngine;
struct WaveLayout;
struct MobModel;
class MobSpawner;
class MobMover;

/// @brief is responsible for a wave
class WaveEngine
{
public:
	WaveEngine(LogicEngine &logic_p);
	~WaveEngine();

	void waveLoop(WaveLayout const &layout_p);

	void handleFrame(double elapsedTime_p);

	void clearUp();

	void spawnMob(MobModel const &model_p, std::array<double, 2> const & spawnPosition_p, double spawntime_p);
	void despawnMob(MobEntity * entity_p);
	void moveMob(MobEntity * entity_p, std::array<double,2> pos_p, std::array<double,2> dir_p);

	PositionalTree<MobEntity> const &getTree() const { return _tree; }
	PositionalTree<MobEntity> &getTree() { return _tree; }

	double getTimeStamp() const { return _timestamp; }

protected:
	LogicEngine &_logic;

	bool isWaveOver();

	/// @brief handle massive entity count positions
	PositionalTree<MobEntity> _tree;

	std::list<MobEntity *> _mobs;

	double _timestamp;

	MobSpawner * _spawner;
	MobMover * _mover;
	bool _spawnOver;
};
