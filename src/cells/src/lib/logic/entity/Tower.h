#pragma once

#include <list>

#include "LogicEntityDefs.h"

#include "logic/types/DamageStyle.h"
#include "logic/types/DamageType.h"

#include "logic/slot/AttackModifier.h"

class Slot;

class Tower : public LogicEntity<Tower>
{
public:
	Tower(std::array<double, 2> const &position_p, std::array<double, 2> const &size_p);

	double getMultAttackSpeed() const { return _multAttackSpeed; }
	void setMultAttackSpeed(double multAttackSpeed_p) { _multAttackSpeed = multAttackSpeed_p; }

	double getMultDamage() const { return _multDamage; }
	void setMultDamage(double multDamage_p) { _multDamage = multDamage_p; }

	double getBonusRange() const { return _bonusRange; }
	void setBonusRange(double bonusRange_p) { _bonusRange = bonusRange_p; }

	DamageStyle getDamageEffect() const { return _damageEffect; }
	void setDamageEffect(DamageStyle damageEffect_p) { _damageEffect = damageEffect_p; }

	unsigned long getMaxSlots() const { return _maxSlots; }
	void setMaxSlots(unsigned long maxSlots_p) { _maxSlots = maxSlots_p; }

	AttackModifier const & getAttackModifier() const { return _attackModifier; }
	void setAttackModifier(AttackModifier const & modifier_p) { _attackModifier = modifier_p; }

	std::list<Slot *> const & getSlots() const { return _slots; }
	std::list<Slot *> & getSlots() { return _slots; }

	/// @brief get full attack speed (taking multiplier into account)
	double getAttackSpeed() const;
	/// @brief get full damages (taking multiplier into account)
	double getDamage() const;
	/// @brief get full ranges (taking multiplier into account)
	double getRange() const;

	std::string const &getResource() const { return _resource; }
	void setResource(std::string const &resource_p) { _resource = resource_p; }

protected:
	double _multAttackSpeed;
	double _multDamage;
	double _bonusRange;
	DamageStyle _damageEffect;
	unsigned long _maxSlots;
	AttackModifier _attackModifier;

	std::list<Slot *> _slots;

	/// @brief graphic resource
	std::string _resource;
};
