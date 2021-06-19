#include "PopUpScreen.h"

#include "engine/GraphicEngine.h"

#include "ColibriGui/Layouts/ColibriLayoutLine.h"

PopUpScreen::PopUpScreen()
	: _manager(nullptr)
	, _window(nullptr)
	, _buttonWindow(nullptr)
{}

void PopUpScreen::init(std::vector<ButtonData> const &data_p, GraphicEngine & engine_p)
{
	_manager = engine_p.getColibriManager();
	_window = _manager->createWindow(nullptr);

	// Adjust window to the center
	_window->setTransform( Ogre::Vector2( _manager->getCanvasSize().x / 2 - 225 , _manager->getCanvasSize().y / 2 - (data_p.size()+1) * 34.5 )
		, Ogre::Vector2( 450, 50 ) );
	float borders_l[4] = {0, 0, 0, 0};
	_window->setBorderSize(borders_l);
	_window->m_breadthFirst = true;

	_buttonWindow = _manager->createWindow(nullptr);
	_buttonWindow->setBorderSize(borders_l);
	_buttonWindow->m_breadthFirst = true;
	_buttonWindow->setVisualsEnabled(false);

	Colibri::LayoutLine *layout_l = new Colibri::LayoutLine(_manager);
	layout_l->m_vertical = false;

	for(ButtonData const & buttonData_l : data_p)
	{
		Colibri::Button *button_l = _manager->createWidget<Colibri::Button>(_buttonWindow);
		button_l->m_minSize = Ogre::Vector2( 350, 64 );
		button_l->getLabel()->setText(buttonData_l.text);
		button_l->sizeToFit();
		layout_l->addCell(button_l);
		_buttons.push_back(button_l);

		// event
		if(buttonData_l.listener)
		{
			_listeners.push_back(new ::WidgetListener(button_l, buttonData_l.listener));
			button_l->addActionListener(_listeners.back());
			_listenersFunctor.push_back(buttonData_l.listener);
		}
		if(!buttonData_l.skin.empty())
		{
			button_l->setSkin(buttonData_l.skin);
		}

		// tooltip
		if(buttonData_l.tooltip)
		{
			_listeners.push_back(new ::TooltipListener(_manager, button_l, buttonData_l.tooltip));
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

	layout_l->setAdjustableWindow(_buttonWindow);
	layout_l->m_hardMaxSize = _manager->getCanvasSize();
	layout_l->layout();

	setContent();

	_window->setSize(Ogre::Vector2(std::max(_window->getSize().x, _buttonWindow->getSize().x), _window->getSize().y));
	// Adjust window to the center
	_window->setTransform( Ogre::Vector2( _manager->getCanvasSize().x / 2. - _window->getSize().x /2.
		, _manager->getCanvasSize().y / 2. -  _window->getSize().y/2. )
		, _window->getSize());
	_buttonWindow->setTransform( Ogre::Vector2( _manager->getCanvasSize().x / 2. - _buttonWindow->getSize().x /2.
		, _window->getSize().y + _window->getLocalTopLeft().y + 5. )
		, _buttonWindow->getSize());

	// no scroll here!
	_window->setMaxScroll(Ogre::Vector2(0,0));
	_window->update(0.f);
	_buttonWindow->setMaxScroll(Ogre::Vector2(0,0));
	_buttonWindow->update(0.f);
}

void PopUpScreen::clean()
{
	if(_window)
	{
		_manager->destroyWindow(_window);
	}
	_window = nullptr;
	if(_buttonWindow)
	{
		_manager->destroyWindow(_buttonWindow);
	}
	_buttonWindow = nullptr;
}

PopUpScreen::~PopUpScreen()
{
	clean();
	// delete all listeners
	for(Colibri::WidgetActionListener* listener_l : _listeners)
	{
		delete listener_l;
	}
	// delete all listeners
	for(Listener * listener_l : _listenersFunctor)
	{
		delete listener_l;
	}
}

void PopUpScreen::setHidden(bool hidden_p)
{
	_window->setHidden(hidden_p);
	_window->setClickable(!hidden_p);
	_buttonWindow->setHidden(hidden_p);
	_buttonWindow->setClickable(!hidden_p);
	for(Colibri::Button * button_l : _buttons)
	{
		button_l->setClickable(!hidden_p);
		button_l->setKeyboardNavigable(!hidden_p);
	}
}
