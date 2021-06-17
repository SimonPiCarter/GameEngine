#pragma once

#include <array>

#include "wave/PositionalTreeDefs.h"
#include "entity/MobEntity.h"

class AttackBuilder;
class Effect;
class LogicEngine;
class MobMover;
class MobSpawner;
struct MobModel;
struct WaveLayout;

/// @brief is responsible for a wave
class WaveEngine
{
public:
	WaveEngine(LogicEngine &logic_p);
	~WaveEngine();

	void init(WaveLayout const &layout_p);
	void handleFrame(double elapsedTime_p);

	void clearUp();

	void spawnMob(MobModel const &model_p, std::array<double, 2> const & spawnPosition_p, double spawntime_p);
	void despawnMob(MobEntity * entity_p);
	void killMob(MobEntity * entity_p);
	void moveMob(MobEntity * entity_p, std::array<double,2> pos_p, std::array<double,2> dir_p);

	LogicEngine &getLogic() { return _logic; }
	PositionalTree<MobEntity> const &getTree() const { return _tree; }
	PositionalTree<MobEntity> &getTree() { return _tree; }

	double getTimeStamp() const { return _timestamp; }

	void addEffect(Effect * effect_p) { _effects.push_back(effect_p); }
	std::list<Effect *> const & getEffects() { return _effects; }

	bool isWaveOver();
protected:
	LogicEngine &_logic;


	/// @brief handle massive entity count positions
	PositionalTree<MobEntity> _tree;

	std::list<MobEntity *> _mobs;
	std::list<Effect *> _effects;

	double _timestamp;

	unsigned long _spawnedMob;
	unsigned long _unspawnedMob;

	MobSpawner * _spawner;
	MobMover * _mover;
	AttackBuilder * _attack;
	bool _spawnOver;
};
