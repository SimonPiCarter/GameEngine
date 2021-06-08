#pragma once

#include "layout/MapLayout.h"

class CellsEngine;
class GraphicEntity;
class HeaderUI;
class MobEntity;
class Tower;
class WaveEngine;

class LogicEngine
{
public:
	LogicEngine(MapLayout const * map_p, CellsEngine * cellsEngine_p=nullptr);
	~LogicEngine();

	CellsEngine * getCellsEngine() { return _cellsEngine; }
	CellsEngine const * getCellsEngine() const { return _cellsEngine; }

	void quit() { _quit = true; }

	double getLife() const { return _life; }
	double getScrap() const { return _scrap; }

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

	MobEntity * getMobSelection(std::array<double, 3> pos_p, std::array<double, 3> dir_p);

protected:
	CellsEngine * const _cellsEngine;

	HeaderUI * _header;

	bool _quit;

	double _life;
	double _scrap;

	/// @brief map
	MapLayout const * const _currentMap;

	/// @brief list of tower
	std::list<Tower *> _towers;

	std::list<std::pair<GraphicEntity *, double> > _particles;

	std::list<GraphicEntity *> _lights;

	WaveEngine * _waveEngine;

	/// @brief tandem class can access logic data
	friend class WaveEngine;
};
