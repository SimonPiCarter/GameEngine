#pragma once

#include "layout/MapLayout.h"

class WaveEngine;

class LogicEngine
{
public:
	LogicEngine();

	void quit() { _quit = true; }

	void setCurrentMap(MapLayout const * map_p) { _currentMap = map_p; }

	void run();
protected:
	bool _quit;

	MapLayout const * _currentMap;

	/// @brief tandem class can access logic data
	friend class WaveEngine;
};
