#include "WaveEngine.h"

#include <chrono>
#include <cassert>

#include "logic/LogicEngine.h"
#include "logic/layout/MobModelLayout.h"
#include "logic/wave/MobSpawner.h"
#include "logic/wave/MobMover.h"
#include "logic/wave/AttackBuilder.h"
#include "logic/effect/Effect.h"

WaveEngine::WaveEngine(LogicEngine &logic_p)
	: _logic(logic_p)
	, _tree({{0.,0.}, {double(_logic._currentMap->getSize()[0]), double(_logic._currentMap->getSize()[1])}}, 20)
	, _timestamp(0.)
	, _spawner(nullptr)
	, _mover(nullptr)
	, _attack(nullptr)
	, _spawnOver(false)
{}

WaveEngine::~WaveEngine()
{
	clearUp();
}

void WaveEngine::init(WaveLayout const &layout_p)
{
	_timestamp = 0.;

	clearUp();

	_spawner = new MobSpawner(*this, layout_p, *_logic._currentMap);
	_spawnOver = false;
	_mover = new MobMover(*this);
	_attack = new AttackBuilder(*this);

}

void handleEffect(Effect * effect_l, std::list<Effect *> & list_r, double elapsedTime_p, double timestamp_p)
{
	EffectStatus status_l = effect_l->apply(std::min(elapsedTime_p, timestamp_p - effect_l->getSpawntime()));

	if(status_l.isOver)
	{
		delete effect_l;
	}
	else
	{
		list_r.push_back(effect_l);
	}
	// recursive call on poped effects using the remaining time
	for(auto &pair_l : status_l.newEffects)
	{
		handleEffect(pair_l.first, list_r, pair_l.second, timestamp_p);
	}
}

void WaveEngine::handleFrame(double elapsedTime_p)
{
	_timestamp += elapsedTime_p;

	assert(_spawner);
	// Spawn mob
	if(!_spawnOver)
	{
		_spawnOver = _spawner->spawn(elapsedTime_p);
	}

	// Compute mob movement
	// Apply movement and update octrees
	_mover->moveEntities(_mobs, elapsedTime_p);

	// Deduce life
	for(MobEntity * entity_l : _tree.getAllWithinRadius(_logic._currentMap->getTargetPoint(), 1e-3))
	{
		// TODO send message : lost life

		despawnMob(entity_l);
	}

	// Trigger attack
	_attack->buildAttacks(elapsedTime_p);

	// Resolve effects
	std::list<Effect *> newEffects_l;
	for(Effect * effect_l : _effects)
	{
		handleEffect(effect_l, newEffects_l, elapsedTime_p, _timestamp);
	}
	// swap effects lists
	std::swap(newEffects_l, _effects);

}

void WaveEngine::clearUp()
{
	for(MobEntity * ent_l : _mobs)
	{
		delete ent_l;
	}
	_mobs.clear();
	delete _spawner;
	delete _mover;
	delete _attack;
}


void WaveEngine::spawnMob(MobModel const &model_p, std::array<double, 2> const & spawnPosition_p, double spawntime_p)
{
	assert(_logic._currentMap);
	_mobs.push_back(new MobEntity(spawnPosition_p, &model_p, *_logic._currentMap, spawntime_p));
	_tree.addContent(_mobs.back());

	// Todo send message
}

void WaveEngine::despawnMob(MobEntity * entity_p)
{
	_tree.removeContent(entity_p);

	// Todo send message
}

void WaveEngine::moveMob(MobEntity * entity_p, std::array<double,2> pos_p, std::array<double,2> dir_p)
{
	_tree.updatePositionFromNode(*entity_p, pos_p);
	// upatePositionFromNode require the position to still be at its old state
	// therefore we update position after
	entity_p->setPosition(pos_p);

	// Todo send message
}

bool WaveEngine::isWaveOver()
{
	return _spawnOver && _mobs.empty();
}
