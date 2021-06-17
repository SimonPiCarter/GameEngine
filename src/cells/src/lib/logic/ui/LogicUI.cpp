#include "LogicUI.h"

#include "HeaderUI.h"
#include "MobSelectionUI.h"
#include "TowerSelectionUI.h"
#include "InventoryUI.h"

LogicUI::LogicUI()
	: _header(nullptr)
	, _mobSelectionUI(nullptr)
	, _towerSelectionUI(nullptr)
	, _inventoryUI(nullptr)
{}
LogicUI::~LogicUI()
{
	delete _header;
	delete _mobSelectionUI;
	delete _towerSelectionUI;
	delete _inventoryUI;
}

void LogicUI::update()
{
	if(_header)
	{
		_header->update();
	}
	if(_mobSelectionUI)
	{
		_mobSelectionUI->update();
	}
	if(_towerSelectionUI)
	{
		_towerSelectionUI->update();
	}
	if(_inventoryUI)
	{
		_inventoryUI->update();
	}
}
