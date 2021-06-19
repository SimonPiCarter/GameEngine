#include "Slow.h"

#include <cassert>
#include "logic/entity/MobEntity.h"

Slow::Slow(MobEntity * target_p, std::string id_p, double spawntime_p, double duration_p, double slow_p, unsigned long maxStack_p)
	: BuffTimer(target_p, id_p, spawntime_p, duration_p, maxStack_p)
	, _slow(slow_p)
{
	assert(_slow > 1e-5);
	assert(_slow < 1.-1e-5);
}

void Slow::applyBuff()
{
	_mobTarget->setSpeed(_mobTarget->getSpeed() * (1.-std::min(0.99,(_slow*_stack))));
}

void Slow::revertBuff()
{
	_mobTarget->setSpeed(_mobTarget->getSpeed() * (1.-std::min(0.99,(_slow*_stack))));
}
