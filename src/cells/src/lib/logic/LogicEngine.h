#pragma once

#include "layout/MapLayout.h"
#include "wave/PositionalTreeDefs.h"

#include <set>

class CellsEngine;
class GraphicEntity;
class HeaderUI;
class MobEntity;
class MobSelectionUI;
class InventoryUI;
class TowerSelectionUI;
class Tower;
class WaveEngine;
class Slot;

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
	double getTime() const { return _time; }

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
	Tower * getTowerSelection(std::array<double, 3> pos_p, std::array<double, 3> dir_p);

	void setMobSelection(MobEntity *ent_p) { _mobSelection = ent_p; }
	MobEntity * getMobSelection() { return _mobSelection; }

	void setTowerSelection(Tower *ent_p) { _towerSelection = ent_p; }
	Tower * getTowerSelection() { return _towerSelection; }

	std::vector<Slot *> & getInventorySlots() { return _inventorySlots; }
	std::vector<Slot *> const & getInventorySlots() const { return _inventorySlots; }

	void setInventoryHidden(bool hidden_p);
	bool isInventoryHidden();

	void updateInventory(std::set<Slot *> const &consumedSlots_p);
	void deleteSlots(std::set<Slot *> const &toBeRemovedSlots_p);

protected:
	CellsEngine * const _cellsEngine;

	HeaderUI * _header;
	MobSelectionUI * _mobSelectionUI;
	TowerSelectionUI * _towerSelectionUI;
	InventoryUI * _inventoryUI;
	bool _inventoyHidden;

	/// @brief handle towers
	PositionalTree<Tower> _tree;

	bool _quit;

	double _life;
	double _scrap;
	double _time;

	/// @brief currently selected mob
	MobEntity * _mobSelection;
	/// @brief currently selected tower
	Tower * _towerSelection;

	/// @brief slot inventory
	std::vector<Slot *> _inventorySlots;

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
