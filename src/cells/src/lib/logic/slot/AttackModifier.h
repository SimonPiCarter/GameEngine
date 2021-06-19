#pragma once

#include "logic/types/AttackType.h"
#include "logic/types/DamageType.h"

#include "gui/InfoLabel.h"
#include "Slot.h"

#include <vector>

class AttackModifier : public Slot
{
public:
	AttackModifier(std::string const & id_p
			, unsigned long lvl_p
			, double speed_p
			, double damage_p
			, double range_p
			, double param_p
			, AttackType attackType_p
			, DamageType damageType_p)
	: Slot(id_p, lvl_p)
	, _speed(speed_p)
	, _damage(damage_p)
	, _range(range_p)
	, _param(param_p)
	, _attackType(attackType_p)
	, _damageType(damageType_p) {}

	/// @brief return true iif Slot is of type EffectMaker
	virtual bool isEffectMaker() const final { return false; }
	/// @brief return true iif Slot is of type Buff
	virtual bool isBuffMaker() const final { return false; }
	/// @brief return true iif Slot is of type AttackModifier
	virtual bool isAttackModifier() const final { return true; }

	double getSpeed() const { return _speed; }
	double getDamage() const { return _damage; }
	double getRange() const { return _range; }
	double getParam() const { return _param; }
	AttackType getAttackType() const { return _attackType; }
	DamageType getDamageType() const { return _damageType; }

	/// @brief return the skin associated with the modifier
	std::string getSkin() const;

	/// @brief return a text description for the modifier
	std::vector<InfoLabel> getDesc() const;

protected:
	double _speed;
	double _damage;
	double _range;
	// used for attack type param (radius for splash; nb of target for arc)
	double _param;
	AttackType _attackType;
	DamageType _damageType;
};
