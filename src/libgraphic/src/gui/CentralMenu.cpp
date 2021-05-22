#include "CentralMenu.h"

#include "engine/GraphicEngine.h"

#include "ColibriGui/ColibriLabel.h"
#include "ColibriGui/ColibriButton.h"
#include "ColibriGui/Layouts/ColibriLayoutLine.h"

using namespace central_menu;

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

Menu::Menu(std::string const &title_p, std::vector<ButtonData> const &data_p, GraphicEngine & engine_p)
{
	_manager = engine_p.getColibriManager();
	_window = _manager->createWindow(nullptr);

	// Adjust window to the center
	_window->setTransform( Ogre::Vector2( _manager->getCanvasSize().x / 2 - 225 , _manager->getCanvasSize().y / 2 - (data_p.size()+1) * 34.5 )
		, Ogre::Vector2( 450, 0 ) );
	_window->m_breadthFirst = true;

	Colibri::LayoutLine *layout_l = new Colibri::LayoutLine(_manager);

	Colibri::Label *label_l = _manager->createWidget<Colibri::Label>(_window);
	label_l->m_minSize = Ogre::Vector2( 350, 64 );
	label_l->setText(title_p);
	label_l->setTextHorizAlignment(Colibri::TextHorizAlignment::Center);
	label_l->setTextVertAlignment(Colibri::TextVertAlignment::Center);
	layout_l->addCell(label_l);

	for(ButtonData const & buttonData_l : data_p)
	{
		Colibri::Button *button_l = _manager->createWidget<Colibri::Button>(_window);
		button_l->m_minSize = Ogre::Vector2( 350, 64 );
		button_l->getLabel()->setText(buttonData_l.text);
		button_l->sizeToFit();
		layout_l->addCell(button_l);

		// event
		if(buttonData_l.listener)
		{
			_listeners.push_back(new ::WidgetListener(button_l, buttonData_l.listener));
			button_l->addActionListener(_listeners.back());
		}
	}

	// Do some spacing (cf demo...)
	{
		const Colibri::LayoutCellVec &cells_l = layout_l->getCells();
		Colibri::LayoutCellVec::const_iterator end  = cells_l.end();

		for( Colibri::LayoutCellVec::const_iterator itor = cells_l.begin();itor != end ; ++itor)
		{
			(*itor)->m_margin = 5.0f;
			(*itor)->m_expand[0] = true;
		}
	}

	layout_l->setAdjustableWindow(_window);
	layout_l->m_hardMaxSize = _manager->getCanvasSize();
	layout_l->layout();

	// no scroll here!
	_window->setMaxScroll(Ogre::Vector2(0,0));
}

Menu::~Menu()
{
	// delete all listeners
	for(WidgetListener* listener_l : _listeners)
	{
		delete listener_l;
	}
	_manager->destroyWindow(_window);
}
