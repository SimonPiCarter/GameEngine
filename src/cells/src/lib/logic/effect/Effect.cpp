#include "Effect.h"

#include "logic/entity/MobEntity.h"
#include "logic/entity/Tower.h"

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

	// multiplier based on damage and armore types
	double mult_l = 1.;
	// Standard vs Heavy : 50%
	// Standard vs Light : 150%
	if(_tower.getAttackModifier().getDamageType() == DamageType::Standard)
	{
		if(_target->getArmorType() == ArmorType::Heavy)
		{
			mult_l = 0.5;
		} else if(_target->getArmorType() == ArmorType::Light)
		{
			mult_l = 1.5;
		}
	}
	// Pearcent vs Heavy : 150%
	// Pearcent vs Light : 50%
	if(_tower.getAttackModifier().getDamageType() == DamageType::Pearcent)
	{
		if(_target->getArmorType() == ArmorType::Heavy)
		{
			mult_l = 1.5;
		} else if(_target->getArmorType() == ArmorType::Light)
		{
			mult_l = 0.5;
		}
	}
	// Corrosive vs Resilient : 50%
	// Corrosive vs Light : 150%
	// Corrosive vs Heavy : 150%
	if(_tower.getAttackModifier().getDamageType() == DamageType::Corrosive)
	{
		if(_target->getArmorType() == ArmorType::Resilient)
		{
			mult_l = 0.5;
		} else if(_target->getArmorType() == ArmorType::Light)
		{
			mult_l = 1.5;
		} else if(_target->getArmorType() == ArmorType::Heavy)
		{
			mult_l = 1.5;
		}
	}

	_target->setHitpoint(_target->getHitpoint() - mult_l * _damage);

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
