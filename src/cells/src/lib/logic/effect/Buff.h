#pragma once

#include <string>
#include "Effect.h"

class MobEntity;
class Tower;

class Buff : public Effect
{
public:
	Buff(MobEntity * target_p, std::string id_p, double spawntime_p) : Effect(spawntime_p), _mobTarget(target_p), _towerTarget(nullptr), _id(id_p) {}
	Buff(Tower * target_p, std::string id_p, double spawntime_p) : Effect(spawntime_p), _mobTarget(nullptr), _towerTarget(target_p), _id(id_p) {}

	std::string const &getId() const { return _id; }

	/// @brief extends this method to compute the effect of the buff and return true if the buff is over
	virtual EffectStatus apply(double elapsedTime_p) = 0;

	/// @brief this method will be called when the buff is applied again on the same target
	/// this may reset a timer or increase a stack for example
	virtual void refresh() = 0;

	virtual void applyBuff() = 0;
	virtual void revertBuff() = 0;

	/// @brief to be called to register the buff in the target
	/// MUST BE CALLED when buff is applied
	void registerBuff();
	/// @brief unregister the buff from the target
	/// MUST BE CALLED when buff is reverted
	void unregisterBuff();
	/// @brief check if buff is already registered in target
	bool isRegistered();
	/// @brief return ref to buff with same id registered in the target
	/// @throw if none
	Buff & getRegisteredBuff();

protected:
	/// by construction only one target can be non null
	/// @brief target of the buff
	MobEntity * const _mobTarget;
	/// @brief target of the buff
	Tower * const _towerTarget;

	/// @brief unique id used to know if the buff is already applied to the target or not
	std::string const _id;
};

class BuffTimer : public Buff
{
public:
	BuffTimer(MobEntity * target_p, std::string id_p, double spawntime_p, double duration_p, unsigned long maxStack_p=1);
	BuffTimer(Tower * target_p, std::string id_p, double spawntime_p, double duration_p, unsigned long maxStack_p=1);

	/// @brief consume buff time and evertually pop it
	virtual EffectStatus apply(double elapsedTime_p);

	/// @brief increment stack if possible and reset timer
	virtual void refresh();

protected:
	/// @brief the duration of the buff
	double const _duration;
	/// @brief max stack for this buff (1 for not stackable)
	unsigned long const _maxStack;
	/// @brief time elapsed since application of the buff
	double _fullElapsedTime;
	/// @brief true if the buff has been applied
	bool _applied;
	/// @brief current stack count
	unsigned long _stack;
};
