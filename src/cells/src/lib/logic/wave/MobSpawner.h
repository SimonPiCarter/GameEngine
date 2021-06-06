#pragma once

#include <list>

#include "logic/layout/WaveLayout.h"

class WaveEngine;
class MapLayout;
struct MobModelLayout;

class MobSpawner
{
public:
	MobSpawner(WaveEngine &waveEngine_p, WaveLayout const &layout_p, MapLayout const &map_p);

	/// @brief
	/// @return true if spawning is over
	bool spawn(double elapsedTime_p);
private:
	WaveEngine &_waveEngine;
	WaveLayout const _layout;
	MapLayout const &_map;
	/// @brief remaining time to spawn next mob
	double _timeToNext;
	/// @brief index of next mob in the current MobModelLayout
	/// 0 means no mob has been spawned from current layout
	unsigned long _indexNext;

	/// @brief iterator to the current layout to spawn
	std::list<MobModelLayout>::const_iterator _currentLayout;

	/// @brief incerement current layout if necessary
	/// reset _indexNext when changing layout
	/// @return true if there is a layout to spawn left
	bool nextLayoutIfNecessary();
};
