#include "ButtonData.h"
#include "RichLabel.h"

#include "ColibriGui/ColibriManager.h"

WidgetListener::WidgetListener(Colibri::Button *button_p, Listener *listener_p)
	: _button(button_p), _listener(listener_p)
{
	assert(_button);
	assert(_listener);
}

WidgetListener::~WidgetListener() {}
void WidgetListener::notifyWidgetAction( Colibri::Widget *widget, Colibri::Action::Action action )
{
	if(action == Colibri::Action::Action::PrimaryActionPerform
	&& widget == _button)
	{
		_listener->run();
	}
}

TooltipListener::TooltipListener(Colibri::ColibriManager * manager_p, Colibri::Button *button_p, RichLabel * label_p)
	: _manager(manager_p), _button(button_p), _label(label_p)
{
	assert(_manager);
	assert(_button);
	assert(_label);
	_label->setHidden(true);
}

TooltipListener::~TooltipListener() {}
void TooltipListener::notifyWidgetAction( Colibri::Widget *widget, Colibri::Action::Action action )
{
	if(widget != _button)
	{
		return;
	}
	if(action == Colibri::Action::Action::Highlighted)
	{
		Ogre::Vector2 posNdc_l = _manager->getMouseCursorPosNdc();
		Ogre::Vector2 size_l = _manager->getCanvasSize();

		_label->setPosition(posNdc_l.x * size_l.x / 2. + size_l.x/2., posNdc_l.y * size_l.y / 2. + size_l.y/2.);
		_label->setHidden(false);
	}
	if(action == Colibri::Action::Action::Cancel
	|| action == Colibri::Action::Action::PrimaryActionPerform)
	{
		_label->setHidden(true);
	}
}
