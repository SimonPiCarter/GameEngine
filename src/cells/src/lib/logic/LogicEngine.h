#pragma once

#include "layout/MapLayout.h"

class CellsEngine;
class GraphicEntity;
class MobEntity;
class Tower;
class WaveEngine;

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

	void spawnMob(MobEntity * entity_p, std::array<double, 2> const & spawnPosition_p);
	void killMob(MobEntity * entity_p);
	void despawnMob(MobEntity * entity_p);
	void moveMob(MobEntity * entity_p, std::array<double,2> oldPos_p, std::array<double,2> pos_p);

	void spawnDamageParticle(std::array<double,2> pos_p, double lifetime_p);

	void spawnTower(Tower * tower_p);
protected:
	CellsEngine * const _cellsEngine;

	bool _quit;

	/// @brief map
	MapLayout const * const _currentMap;

	/// @brief list of tower
	std::list<Tower *> _towers;

	std::list<std::pair<GraphicEntity *, double> > _particles;

	GraphicEntity *_light;

	WaveEngine * _waveEngine;

	/// @brief tandem class can access logic data
	friend class WaveEngine;
};
