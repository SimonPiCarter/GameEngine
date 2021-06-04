#pragma once

#include "Listener.h"

#include <list>
#include <string>
#include "ColibriGui/ColibriButton.h"
#include "ColibriGui/ColibriLabel.h"
#include "ColibriGui/ColibriManager.h"
#include "ColibriGui/ColibriWindow.h"

class GraphicEngine;

namespace central_menu
{

/// @brief This class trigger a Listener when the given button is clicked on
class WidgetListener : public Colibri::WidgetActionListener
{
public:
	WidgetListener(Colibri::Button *button_p, Listener *listener_p);

	virtual ~WidgetListener();
	virtual void notifyWidgetAction( Colibri::Widget *widget, Colibri::Action::Action action );

private:
	Colibri::Button * const _button;
	Listener * const _listener;
};

/// @brief simple definition of a central_menu button
struct ButtonData
{
	std::string text = "";
	Listener * listener = nullptr;
};


class Menu
{
public:
	Menu(std::string const &title_p, std::vector<ButtonData> const &data_p, GraphicEngine & engine_p);
	virtual ~Menu();

	void setHidden(bool hidden_p);

protected:
	Colibri::ColibriManager * _manager;
	Colibri::Window * _window;
	std::list<Colibri::Button *> _buttons;
	Colibri::Label * _label;

	/// @brief list of listeners to delete them
	std::list<WidgetListener*> _listeners;
};

}
