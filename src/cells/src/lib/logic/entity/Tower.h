#pragma once

#include <list>

#include "logic/types/DamageStyle.h"
#include "logic/types/DamageType.h"

class AttackModifier;
class Slot;

class Tower
{
public:
	Tower();

	double getBaseAttackSpeed() const { return _baseAttackSpeed; }
	void setBaseAttackSpeed(double baseAttackSpeed_p) { _baseAttackSpeed = baseAttackSpeed_p; }

	double getMultAttackSpeed() const { return _multAttackSpeed; }
	void setMultAttackSpeed(double multAttackSpeed_p) { _multAttackSpeed = multAttackSpeed_p; }

	double getBaseDamage() const { return _baseDamage; }
	void setBaseDamage(double baseDamage_p) { _baseDamage = baseDamage_p; }

	double getMultDamage() const { return _multDamage; }
	void setMultDamage(double multDamage_p) { _multDamage = multDamage_p; }

	double getRange() const { return _range; }
	void setRange(double range_p) { _range = range_p; }

	double getBonusRange() const { return _bonusRange; }
	void setBonusRange(double bonusRange_p) { _bonusRange = bonusRange_p; }

	DamageType getAttackType() const { return _attackType; }
	void setAttackType(DamageType attackType_p) { _attackType = attackType_p; }

	DamageStyle getDamageEffect() const { return _damageEffect; }
	void setDamageEffect(DamageStyle damageEffect_p) { _damageEffect = damageEffect_p; }

	unsigned long getMaxSlots() const { return _maxSlots; }
	void setMaxSlots(unsigned long maxSlots_p) { _maxSlots = maxSlots_p; }

	AttackModifier * getAttackModifier() const { return _attackModifier; }
	void setAttackModifier(AttackModifier * modifier_p) { _attackModifier = modifier_p; }

	std::list<Slot *> const & getSlots() const { return _slots; }
	std::list<Slot *> & getSlots() { return _slots; }

protected:
	double _baseAttackSpeed;
	double _multAttackSpeed;
	double _baseDamage;
	double _multDamage;
	double _range;
	double _bonusRange;
	DamageType _attackType;
	DamageStyle _damageEffect;
	unsigned long _maxSlots;
	AttackModifier * _attackModifier;

	std::list<Slot *> _slots;
};
