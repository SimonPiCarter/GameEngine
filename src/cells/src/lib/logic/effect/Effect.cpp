#include "Effect.h"

#include "logic/entity/MobEntity.h"

Effect::Effect(double spawntime_p) : _spawntime(spawntime_p) {}

DamageEffect::DamageEffect(MobEntity * target_p, Tower &tower_p, double damage_p, double spawntime_p)
	: Effect(spawntime_p)
	, _target(target_p)
	, _tower(tower_p)
	, _damage(damage_p)
{}

EffectStatus DamageEffect::apply(double elapsedTime_p)
{
	EffectStatus status_l;
	status_l.isOver = true;

	_target->setHitpoint(_target->getHitpoint() - _damage);

	return status_l;
}

CompositeEffect::CompositeEffect(std::list<Effect *> const &effects_p, double spawntime_p)
	: Effect(spawntime_p)
	, _effects(effects_p)
{}

EffectStatus CompositeEffect::apply(double elapsedTime_p)
{
	EffectStatus status_l;
	status_l.isOver = true;

	// add all composite in effects
	for(Effect * effect_l : _effects)
	{
		status_l.newEffects.push_back({effect_l, 0.});
	}

	return status_l;
}
