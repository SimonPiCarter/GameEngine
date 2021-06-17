#pragma once

#include "MobModelLayout.h"

#include <list>

struct WaveLayout
{
	double time;
	std::list<MobModelLayout> mobLayout;
};
