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
	, _tree({{0.,0.}, {double(_logic._currentMap->getSize()[0]), double(_logic._currentMap->getSize()[1])}}, _logic._currentMap->getSize()[0], 0.)
	, _timestamp(0.)
	, _spawnedMob(0)
	, _unspawnedMob(0)
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

	// Disable entity with less or equal than 0
	for(MobEntity * entity_l : _mobs)
	{
		// if no life and not yet disabled
		if(entity_l->getHitpoint() <= 0. && !entity_l->isDisabled())
		{
			killMob(entity_l);
		}
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
	_mobs.back()->setMainHitbox({{-model_p.size[0]/2.,-model_p.size[0]/2., 0.}, {model_p.size[0], model_p.size[1], model_p.height}});
	_tree.addContent(_mobs.back());

	_logic.spawnMob(_mobs.back(), spawnPosition_p);
	++_spawnedMob;
}

void WaveEngine::despawnMob(MobEntity * entity_p)
{
	_tree.removeContent(entity_p);
	_logic.despawnMob(entity_p);
	entity_p->setDisabled(true);
	++_unspawnedMob;
}

void WaveEngine::killMob(MobEntity * entity_p)
{
	_tree.removeContent(entity_p);
	_logic.killMob(entity_p);
	entity_p->setDisabled(true);
	++_unspawnedMob;
}

void WaveEngine::moveMob(MobEntity * entity_p, std::array<double,2> pos_p, std::array<double,2>)
{
	std::array<double,2> oldPos_l = entity_p->getPosition();
	_tree.updatePositionFromNode(*entity_p, pos_p);
	// upatePositionFromNode require the position to still be at its old state
	// therefore we update position after
	entity_p->setPosition(pos_p);

	_logic.moveMob(entity_p, oldPos_l, pos_p);
}

bool WaveEngine::isWaveOver()
{
	return _spawnOver && _unspawnedMob == _spawnedMob;
}
