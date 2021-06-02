#pragma once

#include <list>

class Effect;

struct EffectStatus
{
	/// @brief true of the effect is over
	bool isOver;
	/// @brief list of new effects and their time to be handled
	std::list<std::pair<Effect *, double> > newEffects;
};

class Effect
{
public:
	Effect(double spawntime_p);

	/// @brief apply the effect and return the status
	virtual EffectStatus apply(double elapsedTime_p) = 0;

protected:
	double const _spawntime;
};

class MobEntity;
class Tower;

class DamageEffect : public Effect
{
public:
	DamageEffect(MobEntity * target_p, Tower &tower_p, double damage_p, double spawntime_p);

	EffectStatus apply(double elapsedTime_p);

protected:
	MobEntity * const _target;
	Tower & _tower;
	double const _damage;
};


class CompositeEffect : public Effect
{
public:
	CompositeEffect(std::list<Effect *> const &effects_p, double spawntime_p);

	EffectStatus apply(double elapsedTime_p);

protected:
	std::list<Effect *> _effects;
};
