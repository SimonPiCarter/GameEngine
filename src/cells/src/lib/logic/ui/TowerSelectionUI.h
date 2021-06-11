#pragma once

#include "message/gui/CustomGuiMessage.h"

#include "ColibriGui/ColibriManager.h"
#include "ColibriGui/ColibriWindow.h"
#include "gui/RichLabel.h"
#include "message/gui/RichLabelMessage.h"

#include <list>
#include <vector>

class LogicEngine;
class Tower;

void init(CustomGuiToolkit * toolkit_p, GraphicEngine * graphic_p);
void hide(CustomGuiToolkit * toolkit_p, GraphicEngine * graphic_p);
void show(CustomGuiToolkit * toolkit_p, GraphicEngine * graphic_p);

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
	Colibri::Window * _title;
	Colibri::Window * _conf;
	Colibri::Window * _slots;
	RichLabelVessel _stats;
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

	friend void init(CustomGuiToolkit * toolkit_p, GraphicEngine * graphic_p);
	friend void hide(CustomGuiToolkit * toolkit_p, GraphicEngine * graphic_p);
	friend void show(CustomGuiToolkit * toolkit_p, GraphicEngine * graphic_p);
};


