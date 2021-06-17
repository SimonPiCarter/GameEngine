#pragma once

class HeaderUI;
class MobSelectionUI;
class InventoryUI;
class TowerSelectionUI;

class LogicUI
{
public:
	LogicUI();
	~LogicUI();

	void update();

	HeaderUI * _header;
	MobSelectionUI * _mobSelectionUI;
	TowerSelectionUI * _towerSelectionUI;
	InventoryUI * _inventoryUI;
};
