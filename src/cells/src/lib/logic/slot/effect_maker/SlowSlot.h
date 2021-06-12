#pragma once

#include "logic/slot/Slot.h"

class SlowSlot : public EffectMaker
{
public:
	SlowSlot(std::string const & id_p, double slow_p, double duration_p, unsigned long maxStack_p);
	~SlowSlot();

	virtual Effect * newEffect(MobEntity * target_p, Tower const & tower_p, WaveEngine &waveEngine_p, double timestamp_p) const;

	/// @brief return the skin associated with the modifier
	virtual std::string getSkin() const;

	/// @brief return a text description for the modifier
	virtual std::vector<InfoLabel> getDesc() const;

protected:
	double const _slow;
	double const _duration;
	unsigned long const _maxStack;
};
