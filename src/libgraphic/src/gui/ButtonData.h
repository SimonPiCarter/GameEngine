#pragma once

#include "Listener.h"
#include "ColibriGui/ColibriButton.h"

class RichLabel;

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

/// @brief This class trigger a Listener when the given button is clicked on
class TooltipListener : public Colibri::WidgetActionListener
{
public:
	TooltipListener(Colibri::ColibriManager * manager_p, Colibri::Button *button_p, RichLabel * label_p);

	virtual ~TooltipListener();
	virtual void notifyWidgetAction( Colibri::Widget *widget, Colibri::Action::Action action );

private:
	Colibri::ColibriManager * const _manager;
	Colibri::Button * const _button;
	RichLabel * const _label;
};

/// @brief simple definition of a central_menu button
struct ButtonData
{
	std::string text;
	Listener * listener;
	RichLabel * tooltip;
	std::string skin;
};
