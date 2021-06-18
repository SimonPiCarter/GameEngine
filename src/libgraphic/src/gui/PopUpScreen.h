#pragma once

#include "ButtonData.h"

#include <list>
#include <string>
#include "ColibriGui/ColibriLabel.h"
#include "ColibriGui/ColibriManager.h"
#include "ColibriGui/ColibriWindow.h"

class GraphicEngine;

class PopUpScreen
{
public:
	PopUpScreen();
	virtual ~PopUpScreen();

	void init(std::vector<ButtonData> const &data_p, GraphicEngine & engine_p);

	void setHidden(bool hidden_p);

	Colibri::Window * getWindow() { return _window; }

	virtual void setContent() {}

protected:
	Colibri::ColibriManager * _manager;
	Colibri::Window * _window;
	Colibri::Window * _buttonWindow;
	std::list<Colibri::Button *> _buttons;

	/// @brief list of listeners to delete them
	std::list<Colibri::WidgetActionListener*> _listeners;
};
