#pragma once

#include <array>
#include <vector>
#include "gui/InfoLabel.h"

class Effect;
class WaveEngine;
class MobEntity;
class Tower;

class Slot
{
public:
	Slot(std::string const &id_p, unsigned long lvl_p);
	virtual ~Slot();

	/// @brief return true iif Slot is of type EffectMaker
	virtual bool isEffectMaker() const = 0;
	/// @brief return true iif Slot is of type Buff
	virtual bool isBuffMaker() const = 0;
	/// @brief return true iif Slot is of type AttackModifier
	virtual bool isAttackModifier() const = 0;

	/// @brief get lvl
	unsigned long getLvl() const { return _lvl; }

	/// @brief return the skin associated with the modifier
	virtual std::string getSkin() const = 0;

	/// @brief return a text description for the modifier
	virtual std::vector<InfoLabel> getDesc() const = 0;
protected:
	/// @brief unique id of the slot
	std::string _id;
	/// @brief lvl of the slot
	unsigned long _lvl;
};

class EffectMaker : public Slot
{
public:
	EffectMaker(std::string const &id_p, unsigned long lvl_p);
	~EffectMaker();

	/// @brief return true iif Slot is of type EffectMaker
	virtual bool isEffectMaker() const final;
	/// @brief return true iif Slot is of type Buff
	virtual bool isBuffMaker() const final;
	/// @brief return true iif Slot is of type AttackModifier
	virtual bool isAttackModifier() const final;

	virtual Effect * newEffect(MobEntity * target_p, Tower const & tower_p, WaveEngine &waveEngine_p, double timestamp_p) const = 0;
};

/// @brief skin getter for a slot that handles empty slot
std::string getSkin(Slot * slot_p);
/// @brief content getter for a slot that handles empty slot
std::vector<InfoLabel> getDesc(Slot * slot_p);
std::string getLvl(Slot * slot_p);
std::array<double, 3> getLvlColour(Slot * slot_p, bool zeroToOneFormat_p=false);
