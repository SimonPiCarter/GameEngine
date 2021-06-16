#pragma once

#include "message/gui/CustomGuiMessage.h"

#include "ColibriGui/ColibriManager.h"
#include "ColibriGui/ColibriWindow.h"
#include "ColibriGui/Layouts/ColibriLayoutMultiline.h"
#include "ColibriGui/Layouts/ColibriLayoutLine.h"
#include "gui/RichLabel.h"
#include "message/gui/RichLabelMessage.h"

#include "SlotUI.h"

#include <list>
#include <vector>

class LogicEngine;
class Tower;
class Slot;

void init_inventory(CustomGuiToolkit * toolkit_p, GraphicEngine * graphic_p);
void hide_inventory(CustomGuiToolkit * toolkit_p, GraphicEngine * graphic_p);
void show_inventory(CustomGuiToolkit * toolkit_p, GraphicEngine * graphic_p);

class InventoryUI : public CustomGuiToolkit
{
public:
	InventoryUI(LogicEngine &engine_p);
	~InventoryUI();

	/// @brief update render
	void update();

	void setCurrentSlotSelection(SlotUI * selection_p);
	SlotUI * getCurrentSlotSelection() { return _currentSelectedSlot; }
	SlotUI const * getCurrentSlotSelection() const { return _currentSelectedSlot; }

	std::vector<SlotUI *> const &getTowerSlots() const { return _towerSlots; }

	void triggerSave() { _save = true; }
	void triggerReset() { _reset = true; }
	void triggerCancel() { _cancel = true; }

protected:
	LogicEngine &_engine;

	Colibri::ColibriManager * _manager;
	// window to show slots in the inventory
	Colibri::Window * _main;
	Colibri::LayoutMultiline * _layout;
	std::vector<SlotUI *> _slots;

	// window to show the tower configuration (slots)
	Colibri::Window * _towerWindow;
	Colibri::LayoutLine * _towerLayout;
	std::vector<SlotUI *> _towerSlots;

	// window to show the tower configuration (atk modifier)
	Colibri::Window * _towerModifierWindow;
	SlotUI * _towerModifierSlot;

	// window for buttons (cancel, save, reset)
	Colibri::Window * _buttonWindow;
	Colibri::LayoutLine * _buttonLayout;

	// pointer to current selection
	SlotUI * _currentSelectedSlot;

	// ui constants
	std::array<double, 2> const _pos;
	std::array<double, 2> const _size;
	std::array<double, 2> const _posTower;
	std::array<double, 2> const _sizeTower;
	std::array<double, 2> const _posButton;
	std::array<double, 2> const _sizeButton;
	unsigned long const _lines;
	unsigned long const _slotSize;

	/// @brief list of listeners to delete them
	std::vector<Colibri::WidgetActionListener*> _listeners;
	/// @brief list of tooltips label to clear them
	std::vector<RichLabelVessel *> _tooltipsLabel;
	std::vector<RichLabelVessel *> _tooltipsLabelTower;

	/// @brief all buttons to disable them
	std::list<Colibri::Button*> _allButtons;


	/// @brief initial set up of the main window of the inventory
	void setUpMain();
	/// @brief initial set up of the window of the tower setup
	void setUpTower();
	/// @brief initial set up of the button window
	void setUpButtons();

	/// @brief update the inventory from the logic engine
	void updateInventory();

	/// @brief update the tower part of the UI
	void updateTower();

	bool _cancel;
	bool _reset;
	/// @brief when set tu true we are going to save the operation to the game logic during last update call
	bool _save;
	void saveConf();

	friend void init_inventory(CustomGuiToolkit * toolkit_p, GraphicEngine * graphic_p);
	friend void hide_inventory(CustomGuiToolkit * toolkit_p, GraphicEngine * graphic_p);
	friend void show_inventory(CustomGuiToolkit * toolkit_p, GraphicEngine * graphic_p);
};

/// @brief This class trigger a Listener when the given button is clicked on
/// used to select an inventory slot
class SelectionInventoryUIListener : public Colibri::WidgetActionListener
{
public:
	SelectionInventoryUIListener(SlotUI &slot_p, InventoryUI &ui_p);
	virtual ~SelectionInventoryUIListener();

	virtual void notifyWidgetAction( Colibri::Widget *widget, Colibri::Action::Action action );

private:
	SlotUI & _slot;
	InventoryUI &_ui;
};

/// @brief This class trigger a Listener when the given button is clicked on
/// used to select an inventory slot
class SelectionTowerUIListener : public Colibri::WidgetActionListener
{
public:
	SelectionTowerUIListener(SlotUI &slot_p, InventoryUI &ui_p, LogicEngine &engine_p);
	virtual ~SelectionTowerUIListener();

	virtual void notifyWidgetAction( Colibri::Widget *widget, Colibri::Action::Action action );

	void resetOriginSlot() { _originSlot = nullptr; }

private:
	SlotUI & _slot;
	// used to reenable slots ui if necessary
	SlotUI * _originSlot;
	InventoryUI &_ui;
	LogicEngine &_engine;
};

/// @brief This class trigger a Listener when the given button is clicked on
/// used to validate modification
class SaveInventoryListener : public Colibri::WidgetActionListener
{
public:
	SaveInventoryListener(Colibri::Button *button_p, InventoryUI &ui_p);
	virtual ~SaveInventoryListener();

	virtual void notifyWidgetAction( Colibri::Widget *widget, Colibri::Action::Action action );

private:
	Colibri::Button * const _button;
	InventoryUI &_ui;
};

/// @brief This class trigger a Listener when the given button is clicked on
/// used to reset modification
class ResetInventoryListener : public Colibri::WidgetActionListener
{
public:
	ResetInventoryListener(Colibri::Button *button_p, InventoryUI &ui_p);
	virtual ~ResetInventoryListener();

	virtual void notifyWidgetAction( Colibri::Widget *widget, Colibri::Action::Action action );

private:
	Colibri::Button * const _button;
	InventoryUI &_ui;
};

/// @brief This class trigger a Listener when the given button is clicked on
/// used to reset modification
class CancelInventoryListener : public Colibri::WidgetActionListener
{
public:
	CancelInventoryListener(Colibri::Button *button_p, InventoryUI &ui_p);
	virtual ~CancelInventoryListener();

	virtual void notifyWidgetAction( Colibri::Widget *widget, Colibri::Action::Action action );

private:
	Colibri::Button * const _button;
	InventoryUI &_ui;
};
