#include "MobSpawner.h"

#include "logic/WaveEngine.h"
#include "logic/layout/MapLayout.h"
#include "logic/layout/MobModelLayout.h"
#include "logic/layout/WaveLayout.h"

MobSpawner::MobSpawner(WaveEngine &waveEngine_p, WaveLayout const &layout_p, MapLayout const &map_p)
	: _waveEngine(waveEngine_p)
	, _layout(layout_p)
	, _map(map_p)
	, _timeToNext(0.)
	, _indexNext(0)
	, _currentLayout(_layout.mobLayout.cbegin())
{}

/// @brief
/// @return true if spawning is over
bool MobSpawner::spawn(double elapsedTime_p)
{
	// if current layout is done skip to next
	bool done_l = !nextLayoutIfNecessary();
	if(done_l) { return true; }

	// keep track of remaining time while we spawn
	double remainingTime_l = elapsedTime_p;

	// While time is sufficient to spawn something
	while(remainingTime_l >= _timeToNext)
	{
		// spawn
		_waveEngine.spawnMob(_currentLayout->model, _map.getSpawnPoint());

		// increment index
		++_indexNext;

		// update remaining time
		remainingTime_l -= _timeToNext;

		// change layout if necessary
		done_l = !nextLayoutIfNecessary();
		if(done_l) { return true; }

		// update time to next from layout
		_timeToNext = _currentLayout->delay;
	}

	// reduce time to next according to remaining time
	_timeToNext -= remainingTime_l;

	return done_l;
}

/// @brief incerement current layout if necessary
/// reset _indexNext when changing layout
/// @return true if there is a layout to spawn left
bool MobSpawner::nextLayoutIfNecessary()
{
	// increment layout iterator if necessary
	while(_currentLayout != _layout.mobLayout.cend()
	&& _indexNext >= _currentLayout->quantity)
	{
		++_currentLayout;
		// reset index next if we change layout
		_indexNext = 0;
	}
	return _currentLayout != _layout.mobLayout.cend();
}
