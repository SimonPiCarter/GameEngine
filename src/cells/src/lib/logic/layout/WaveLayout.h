#pragma once

#include "MobModelLayout.h"

#include <list>
#include <string>

class Slot;

struct WaveLayout
{
	double time;
	std::list<MobModelLayout> mobLayout;
	std::list<Slot *> rewards;
	std::string rewardText;
};
