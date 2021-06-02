#include "AttackBuilder.h"

#include <logic/effect/Effect.h>
#include <logic/entity/Tower.h>
#include <logic/LogicEngine.h>
#include <logic/slot/Slot.h>
#include <logic/utils/ArrayUtils.h>
#include <logic/WaveEngine.h>

AttackBuilder::AttackBuilder(WaveEngine &waveEngine_p)
	: _waveEngine(waveEngine_p)
{
	// Create AttackInfos
	for(Tower * tower_l : _waveEngine.getLogic().getTowers())
	{
		_attackInfos.push_back({tower_l, 0., nullptr});
	}
}

void AttackBuilder::buildAttacks(double elapsedTime_p)
{
	for(AttackInfo & info_l : _attackInfos)
	{
		double remainingTime_l = elapsedTime_p;
		while(remainingTime_l > 0.)
		{
			// time consumed is minimum between reload time and elapsed time
			double consumed_l = std::min(info_l.reload, remainingTime_l);

			double speed_l = info_l.tower->getAttackModifier().getSpeed();
			// if consumed >= reload we fire
			if(consumed_l >= info_l.reload
			&& speed_l > 0.)
			{
				// build attack with time stamp
				bool hasAttacked_l = buildAttack(info_l, _waveEngine.getTimeStamp()-remainingTime_l);
				// reset reload time
				if(hasAttacked_l)
				{
					info_l.reload = 1. / speed_l;
				}
				// else break loop there is no need to loop another time
				else
				{
					break;
				}
			}

			remainingTime_l -= consumed_l;
		}
	}
}

bool AttackBuilder::buildAttack(AttackInfo &info_p, double timestamp_p)
{
	std::list<MobEntity *> targets_l = findTargets(info_p);

	for(MobEntity * target_l : targets_l)
	{
		Effect * effect_l = buildAttackEffect(target_l, *info_p.tower, timestamp_p);
		_waveEngine.addEffect(effect_l);
	}

	// tower attacked if we found target
	return !targets_l.empty();
}

namespace
{
	bool isInRange(MobEntity * target_p, Tower * tower_p)
	{
		if(target_p == nullptr || tower_p == nullptr)
		{
			return false;
		}
		return square_distance(target_p->getPosition(), tower_p->getPosition()) <= tower_p->getRange();
	}
}

std::list<MobEntity *> AttackBuilder::findTargets(AttackInfo &info_p)
{
	Tower * tower_l = info_p.tower;
	std::list<MobEntity *> targets_l;
	if(tower_l->getAttackModifier().getAttackType() == AttackType::Direct)
	{
		// if no target, dead or not within range
		if(nullptr == info_p.target || info_p.target->getHitpoint() <= 1e-5|| !isInRange(info_p.target, tower_l))
		{
			// aquire new target
			MobEntity * target_l = _waveEngine.getTree().getClosestFromPosition(tower_l->getPosition(), tower_l->getRange());
			info_p.target = target_l;
		}

		if(info_p.target)
		{
			targets_l.push_back(info_p.target);
		}
	}
	return targets_l;
}

Effect * AttackBuilder::buildAttackEffect(MobEntity * target_p, Tower &tower_p, double timestamp_p)
{
	std::list<Effect *> effects_l;
	if(tower_p.getDamageEffect() == DamageStyle::Standard)
	{
		effects_l.push_back(new DamageEffect(target_p, tower_p, tower_p.getDamage(), timestamp_p));

		for(Slot * slot_l : tower_p.getSlots())
		{
			if(slot_l->isEffectMaker())
			{
				EffectMaker const * maker_l = static_cast<EffectMaker const *>(slot_l);
				effects_l.push_back(maker_l->newEffect(target_p, tower_p, _waveEngine, timestamp_p));

			}
		}
	}
	return new CompositeEffect(effects_l, timestamp_p);
}
