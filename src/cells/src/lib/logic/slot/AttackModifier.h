#pragma once

#include "logic/types/AttackType.h"
#include "logic/types/DamageType.h"

class AttackModifier
{
public:
	AttackModifier(double speed_p
			, double damage_p
			, double range_p
			, AttackType attackType_p
			, DamageType damageType_p)
	:_speed(speed_p)
	, _damage(damage_p)
	, _range(range_p)
	, _attackType(attackType_p)
	, _damageType(damageType_p) {}

	double getSpeed() const { return _speed; }
	double getDamage() const { return _damage; }
	double getRange() const { return _range; }
	AttackType getAttackType() const { return _attackType; }
	DamageType getDamageType() const { return _damageType; }

protected:
	double _speed;
	double _damage;
	double _range;
	AttackType _attackType;
	DamageType _damageType;
};
