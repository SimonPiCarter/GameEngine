#include "Buff.h"

#include "logic/entity/MobEntity.h"
#include "logic/entity/Tower.h"

/// @brief to be called to register the buff in the target
/// MUST BE CALLED when buff is applied
void Buff::registerBuff()
{
	if(_mobTarget)
	{
		_mobTarget->registerBuff(this);
	}
	else if(_towerTarget)
	{
		_towerTarget->registerBuff(this);
	}
}

/// @brief unregister the buff from the target
/// MUST BE CALLED when buff is reverted
void Buff::unregisterBuff()
{
	if(_mobTarget)
	{
		_mobTarget->unregisterBuff(this);
	}
	else if(_towerTarget)
	{
		_towerTarget->unregisterBuff(this);
	}
}

bool Buff::isRegistered()
{
	if(_mobTarget)
	{
		return _mobTarget->isRegistered(this);
	}
	else if(_towerTarget)
	{
		return _towerTarget->isRegistered(this);
	}
	return false;
}

Buff & Buff::getRegisteredBuff()
{
	Buff * ptr_l = nullptr;
	if(_mobTarget)
	{
		ptr_l = _mobTarget->getRegisteredBuff(this->getId());
	}
	else if(_towerTarget)
	{
		ptr_l = _towerTarget->getRegisteredBuff(this->getId());
	}
	if(!ptr_l) { throw std::logic_error("Looking for unregistered buff in Buff::getRegisteredBuff()"); }
	return *ptr_l;
}

BuffTimer::BuffTimer(MobEntity * target_p, std::string id_p, double spawntime_p, double duration_p, unsigned long maxStack_p)
	: Buff(target_p, id_p, spawntime_p)
	, _duration(duration_p)
	, _maxStack(maxStack_p)
	, _fullElapsedTime(0.)
	, _applied(false)
	, _stack(0)
{}
BuffTimer::BuffTimer(Tower * target_p, std::string id_p, double spawntime_p, double duration_p, unsigned long maxStack_p)
	: Buff(target_p, id_p, spawntime_p)
	, _duration(duration_p)
	, _maxStack(maxStack_p)
	, _fullElapsedTime(0.)
	, _applied(false)
	, _stack(0)
{}

EffectStatus BuffTimer::apply(double elapsedTime_p)
{
	// if first application
	if(!_applied)
	{
		_applied = true;
		// if not already registered we add it
		if(!isRegistered())
		{
			++_stack;
			// apply buff
			applyBuff();
			registerBuff();
			// add a stack
		}
		// else just refresh
		else
		{
			getRegisteredBuff().refresh();
			// if we have refreshed the other instance of the same buff
			// we delete this sone asap
			EffectStatus status_l;
			status_l.isOver;
			return status_l;
		}
	}

	_fullElapsedTime += elapsedTime_p;

	EffectStatus status_l;
	status_l.isOver = _fullElapsedTime > _duration;

	// if last application
	if(status_l.isOver && _applied)
	{
		revertBuff();
		unregisterBuff();
	}

	return status_l;
}

void BuffTimer::refresh()
{
	if(_stack < _maxStack)
	{
		// revert buff with old stack count
		revertBuff();
		// add a stack
		++_stack;
		// apply back buff with new stack count
		applyBuff();
	}
	// reset elapsed time
	_fullElapsedTime = 0.;
}
