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
			} else
			{
				info_l.reload -= consumed_l;
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

	// if no target, dead or not within range
	if(nullptr == info_p.target || info_p.target->getHitpoint() <= 1e-5|| !isInRange(info_p.target, tower_l))
	{
		// aquire new target
		MobEntity * target_l = _waveEngine.getTree().getClosestFromPosition(tower_l->getPosition(), tower_l->getRange());
		info_p.target = target_l;
	}

	AttackType type_l = tower_l->getAttackModifier().getAttackType();
	if(type_l == AttackType::Direct)
	{
		if(info_p.target)
		{
			targets_l.push_back(info_p.target);
		}
	}
	else if(type_l == AttackType::Splash)
	{
		if(info_p.target)
		{
			std::unordered_set<MobEntity *> all_l = _waveEngine.getTree().getAllWithinRadius(
					info_p.target->getPosition(), tower_l->getAttackModifier().getParam());
			for(MobEntity * ent_l : all_l)
			{
				targets_l.push_back(ent_l);
			}
		}
	}
	else if(type_l == AttackType::Arc)
	{
		if(info_p.target)
		{
			std::unordered_set<MobEntity *> alreadyTag_l;
			std::array<double, 2> pos_l = info_p.target->getPosition();
			for(size_t i = 0 ; double(i) < tower_l->getAttackModifier().getParam() ; ++ i)
			{
				MobEntity * target_l = _waveEngine.getTree().getClosestFromPosition(pos_l, tower_l->getRange(), alreadyTag_l);
				if(target_l)
				{
					targets_l.push_back(target_l);
					alreadyTag_l.insert(target_l);
					pos_l = target_l->getPosition();
				}
				else
				{
					break;
				}
			}
		}
	}
	else if(type_l == AttackType::Line)
	{
		if(info_p.target)
		{
			std::unordered_set<MobEntity *> all_l = _waveEngine.getTree().getAllWithinLine(
					tower_l->getPosition(), info_p.target->getPosition(), tower_l->getRange());
			for(MobEntity * ent_l : all_l)
			{
				targets_l.push_back(ent_l);
			}
		}
	}

	// spawn particles to the targets
	for(MobEntity * ent_l : targets_l)
	{
		_waveEngine.getLogic().spawnDamageParticle(ent_l->getPosition(), 0.5);
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
