#pragma once

#include "logic/effect/Buff.h"

class Slow : public BuffTimer
{
public:
	Slow(MobEntity * target_p, std::string id_p, double spawntime_p, double duration_p, double slow_p, unsigned long maxStack_p);

	virtual void applyBuff();
	virtual void revertBuff();
protected:
	double const _slow;
};
