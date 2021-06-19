#pragma once

#include "MobModelLayout.h"

#include <list>

class Slot;

struct WaveLayout
{
	double time;
	std::list<MobModelLayout> mobLayout;
	std::list<Slot *> rewards;
};
