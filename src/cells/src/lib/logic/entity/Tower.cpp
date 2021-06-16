#include "Tower.h"

#include "logic/slot/Slot.h"

Tower::Tower(std::array<double, 2> const &position_p, std::array<double, 2> const &size_p)
	: LogicEntity<Tower>(position_p, size_p)
	, _multAttackSpeed(1.)
	, _multDamage(1.)
	, _bonusRange(0.)
	, _damageEffect(DamageStyle::Standard)
	, _maxSlots(1)
	, _attackModifier("BasicModifier", 1, 1., 1., 1., 1., AttackType::Direct, DamageType::Standard)
	, _resource("")
{}
Tower::~Tower()
{
	for(Slot *slot_l : _slots)
	{
		delete slot_l;
	}
}

void Tower::setMaxSlots(unsigned long maxSlots_p)
{
	_maxSlots = maxSlots_p;
	// resize slot vector
	_slots.resize(_maxSlots, nullptr);
}

/// @brief get full attack speed (taking multiplier into account)
double Tower::getAttackSpeed() const
{
	return _attackModifier.getSpeed() * _multAttackSpeed;
}

/// @brief get full damages (taking multiplier into account)
double Tower::getDamage() const
{
	return _attackModifier.getDamage() * _multDamage;
}
/// @brief get full ranges (taking multiplier into account)
double Tower::getRange() const
{
	return _attackModifier.getRange() + _bonusRange;
}
