#pragma once

#include "message/gui/CustomGuiMessage.h"

#include "ColibriGui/ColibriManager.h"
#include "ColibriGui/ColibriWindow.h"
#include "ColibriGui/Layouts/ColibriLayoutLine.h"
#include "gui/RichLabel.h"
#include "message/gui/RichLabelMessage.h"

#include <list>
#include <vector>

class LogicEngine;
class Tower;

void init(CustomGuiToolkit * toolkit_p, GraphicEngine * graphic_p);
void hide(CustomGuiToolkit * toolkit_p, GraphicEngine * graphic_p);
void show(CustomGuiToolkit * toolkit_p, GraphicEngine * graphic_p);
void slot(CustomGuiToolkit * toolkit_p, GraphicEngine * graphic_p);

class TowerSelectionUI : public CustomGuiToolkit
{
public:
	TowerSelectionUI(LogicEngine &engine_p);
	~TowerSelectionUI();

	/// @brief update render
	void update();

protected:
	LogicEngine &_engine;
	/// @brief selected tower (used to know when there is a change in selection)
	Tower * _currentSelection;

	Colibri::ColibriManager * _manager;
	// Title of the UI
	Colibri::Window * _title;
	// Button to start configure menu
	Colibri::Window * _conf;
	// window to show slots of the tower
	Colibri::Window * _slots;
	Colibri::LayoutLine * _layout;
	std::vector<Colibri::Button *> _buttons;
	// label that show stats of the twoer
	RichLabelVessel _stats;
	// window and button showing the attack modifier of the tower
	Colibri::Window * _windowModifier;
	Colibri::Button * _modifier;

	std::array<double, 2> _pos;

	/// @brief list of listeners to delete them
	std::list<Colibri::WidgetActionListener*> _listeners;
	/// @brief list of tooltips label to clear them
	/// tooltip[0] is attack modifier
	/// tooltip[i] is the i-1 th slot tooltip
	std::vector<RichLabelVessel *> _tooltipsLabel;

	void setUpTitle();
	void setUpStats();
	void setUpSlots();

	/// @brief update
	void updateSelection(Tower * tower_p);

	void updateSlots();

	friend void init(CustomGuiToolkit * toolkit_p, GraphicEngine * graphic_p);
	friend void hide(CustomGuiToolkit * toolkit_p, GraphicEngine * graphic_p);
	friend void show(CustomGuiToolkit * toolkit_p, GraphicEngine * graphic_p);
	friend void slot(CustomGuiToolkit * toolkit_p, GraphicEngine * graphic_p);
};


