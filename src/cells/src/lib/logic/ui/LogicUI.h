#pragma once

#include "ColibriGui/ColibriManager.h"
#include "message/gui/CustomGuiMessage.h"

#include "gui/ButtonData.h"

class HeaderUI;
class InventoryUI;
class LogicEngine;
class MobSelectionUI;
class TowerSelectionUI;

void init_buildTower(CustomGuiToolkit * toolkit_p, GraphicEngine * graphic_p);

class LogicUI : public CustomGuiToolkit
{
public:
	LogicUI();
	~LogicUI();

	void init(LogicEngine &engine_p);
	void update();

	HeaderUI * _header;
	MobSelectionUI * _mobSelectionUI;
	TowerSelectionUI * _towerSelectionUI;
	InventoryUI * _inventoryUI;

	Colibri::Window *_buildTower;
	ButtonData _buildTowerButton;

	friend void init_buildTower(CustomGuiToolkit * toolkit_p, GraphicEngine * graphic_p);

	/// @brief list of listeners to delete them
	std::vector<Colibri::WidgetActionListener*> _listeners;
};
