#pragma once

#include "ButtonData.h"

#include <list>
#include <string>
#include "ColibriGui/ColibriLabel.h"
#include "ColibriGui/ColibriManager.h"
#include "ColibriGui/ColibriWindow.h"

class GraphicEngine;

namespace central_menu
{
class Menu
{
public:
	Menu(std::string const &title_p, std::vector<ButtonData> const &data_p, GraphicEngine & engine_p);
	virtual ~Menu();

	void setHidden(bool hidden_p);

	Colibri::Window * getWindow() { return _window; }

protected:
	Colibri::ColibriManager * _manager;
	Colibri::Window * _window;
	std::list<Colibri::Button *> _buttons;
	Colibri::Label * _label;

	/// @brief list of listeners to delete them
	std::list<Colibri::WidgetActionListener*> _listeners;
};

}
