#pragma once

#include "layout/MapLayout.h"

class CellsEngine;
class WaveEngine;
class Tower;

class LogicEngine
{
public:
	LogicEngine(MapLayout const * map_p, CellsEngine * cellsEngine_p=nullptr);
	~LogicEngine();

	void quit() { _quit = true; }

	std::list<Tower *> const & getTowers() const { return _towers; }
	std::list<Tower *> & getTowers() { return _towers; }

	void init();
	void run(double elapsedTime_p);
protected:
	CellsEngine * const _cellsEngine;

	bool _quit;

	/// @brief map
	MapLayout const * const _currentMap;

	/// @brief list of tower
	std::list<Tower *> _towers;

	WaveEngine * _waveEngine;

	/// @brief tandem class can access logic data
	friend class WaveEngine;
};
