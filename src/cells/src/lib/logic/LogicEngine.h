#pragma once

#include "layout/MapLayout.h"

class WaveEngine;
class Tower;

class LogicEngine
{
public:
	LogicEngine(MapLayout const * map_p);
	~LogicEngine();

	void quit() { _quit = true; }

	std::list<Tower *> const & getTowers() const { return _towers; }
	std::list<Tower *> & getTowers() { return _towers; }

	void run();
protected:
	bool _quit;

	/// @brief map
	MapLayout const * const _currentMap;

	/// @brief list of tower
	std::list<Tower *> _towers;

	/// @brief tandem class can access logic data
	friend class WaveEngine;
};
