#pragma once

class Effect;
class WaveEngine;
class MobEntity;
class Tower;

class Slot
{
public:
	Slot();
	virtual ~Slot();

	/// @brief return true iif Slot is of type EffectMaker
	virtual bool isEffectMaker() const = 0;
	/// @brief return true iif Slot is of type Buff
	virtual bool isBuffMaker() const = 0;
};

class EffectMaker : public Slot
{
public:
	EffectMaker();
	~EffectMaker();

	/// @brief return true iif Slot is of type EffectMaker
	virtual bool isEffectMaker() const final;
	/// @brief return true iif Slot is of type Buff
	virtual bool isBuffMaker() const final;

	virtual Effect * newEffect(MobEntity const * target_p, Tower const & tower_p, WaveEngine &waveEngine_p, double timestamp_p) const = 0;
};
