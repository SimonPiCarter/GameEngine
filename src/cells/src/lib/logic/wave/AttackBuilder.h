#pragma once

#include <list>

class Effect;
class MobEntity;
class Tower;
class WaveEngine;

struct AttackInfo
{
	/// @brief pointer to the tower
	Tower *tower;
	/// @brief time to next attack
	double reload;
	/// @brief current target of the tower
	MobEntity *target;
};

class AttackBuilder
{
public:
	AttackBuilder(WaveEngine &waveEngine_p);

	void buildAttacks(double elapsedTime_p);
protected:
	WaveEngine &_waveEngine;

	std::list<AttackInfo> _attackInfos;

	/// @brief create an attack from the Tower on the given timestamp
	/// @return true if the tower has indeed attack
	bool buildAttack(AttackInfo &info_p, double timestamp_p);

	std::list<MobEntity *> findTargets(AttackInfo &info_p);

	Effect * buildAttackEffect(MobEntity * target_p, Tower &tower_p, double timestamp_p);
};
