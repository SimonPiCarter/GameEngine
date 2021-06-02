#pragma once

#include <list>

class Effect;

struct EffectStatus
{
	/// @brief true of the effect is over
	bool isOver;
	/// @brief list of new effects and their time to be handled
	std::list<std::pair<Effect *, double> > newEffects;
};

class Effect
{
public:
	Effect();

	/// @brief apply the effect and return the status
	EffectStatus apply(double elapsedTime_p);
};
