#include "LogicUI.h"

#include "HeaderUI.h"
#include "MobSelectionUI.h"
#include "TowerSelectionUI.h"
#include "InventoryUI.h"

#include "logic/LogicEngine.h"
#include "engine/GraphicEngine.h"
#include "game/CellsEngine.h"
#include "gui/ButtonData.h"

void init_buildTower(CustomGuiToolkit * toolkit_p, GraphicEngine * graphic_p)
{
	LogicUI * ui_l = static_cast<LogicUI *>(toolkit_p);

	Colibri::ColibriManager * manager_l = graphic_p->getColibriManager();
	ui_l->_buildTower = manager_l->createWindow(nullptr);
	ui_l->_buildTower->setTransform( Ogre::Vector2(0., manager_l->getCanvasSize().y-160), Ogre::Vector2( 200, 160 ) );
	ui_l->_buildTower->m_breadthFirst = true;
	float borders_l[4] = {0, 0, 0, 0};
	ui_l->_buildTower->setBorderSize(borders_l);

	// button build tower

	std::vector<InfoLabel> content_l;
	content_l.push_back({"build tower", 255, 255, 255});
	ui_l->_buildTowerButton.tooltip = new RichLabel(content_l, 0, 0, 200., 0., 10.f, true, *graphic_p);

	Colibri::Button * button_l = manager_l->createWidget<Colibri::Button>(ui_l->_buildTower);
	button_l->m_minSize = Ogre::Vector2( 200, 80 );

	// event
	if(ui_l->_buildTowerButton.listener)
	{
		ui_l->_listeners.push_back(new WidgetListener(button_l, ui_l->_buildTowerButton.listener));
		button_l->addActionListener(ui_l->_listeners.back());
	}

	if(!ui_l->_buildTowerButton.skin.empty())
	{
		button_l->setSkin(ui_l->_buildTowerButton.skin);
	}
	if(!ui_l->_buildTowerButton.text.empty())
	{
		button_l->getLabel()->setText(ui_l->_buildTowerButton.text);
		button_l->sizeToFit();
	}

	// tooltip
	ui_l->_listeners.push_back(new TooltipListener(manager_l, button_l, ui_l->_buildTowerButton.tooltip));
	button_l->addActionListener(ui_l->_listeners.back());

	// button remove tower

	content_l.clear();
	content_l.push_back({"remove tower", 255, 255, 255});
	ui_l->_removeTowerButton.tooltip = new RichLabel(content_l, 0, 0, 200., 0., 10.f, true, *graphic_p);

	button_l = manager_l->createWidget<Colibri::Button>(ui_l->_buildTower);
	button_l->m_minSize = Ogre::Vector2( 200, 80 );
	button_l->setTransform( Ogre::Vector2(0., 80), button_l->m_minSize );

	// event
	if(ui_l->_removeTowerButton.listener)
	{
		ui_l->_listeners.push_back(new WidgetListener(button_l, ui_l->_removeTowerButton.listener));
		button_l->addActionListener(ui_l->_listeners.back());
	}

	if(!ui_l->_removeTowerButton.skin.empty())
	{
		button_l->setSkin(ui_l->_removeTowerButton.skin);
	}
	if(!ui_l->_removeTowerButton.text.empty())
	{
		button_l->getLabel()->setText(ui_l->_removeTowerButton.text);
		button_l->sizeToFit();
	}

	// tooltip
	ui_l->_listeners.push_back(new TooltipListener(manager_l, button_l, ui_l->_removeTowerButton.tooltip));
	button_l->addActionListener(ui_l->_listeners.back());

}

LogicUI::LogicUI()
	: _header(nullptr)
	, _mobSelectionUI(nullptr)
	, _towerSelectionUI(nullptr)
	, _inventoryUI(nullptr)
	, _buildTower(nullptr)
{}

LogicUI::~LogicUI()
{
	delete _header;
	delete _mobSelectionUI;
	delete _towerSelectionUI;
	delete _inventoryUI;
	delete _buildTowerButton.listener;
	delete _buildTowerButton.tooltip;
	delete _removeTowerButton.listener;
	delete _removeTowerButton.tooltip;
	for(auto ptr_l : _listeners)
	{
		delete ptr_l;
	}
}

namespace
{
	class SwitchToTowerBuild : public Listener
	{
	public:
		SwitchToTowerBuild(LogicEngine &engine_p)
			: _engine(engine_p)
		{}

		void run()
		{
			if(!_engine.isWaveRunning())
			{
				_engine.setPlacingTower(true);
			}
		}
	protected:
		LogicEngine &_engine;
	};
	class RemoveTowerBuild : public Listener
	{
	public:
		RemoveTowerBuild(LogicEngine &engine_p)
			: _engine(engine_p)
		{}

		void run()
		{
			if(!_engine.isWaveRunning())
			{
				_engine.deleteSelectedTower();
			}
		}
	protected:
		LogicEngine &_engine;
	};
} // namespace


void LogicUI::init(LogicEngine &engine_p)
{
	_header = new HeaderUI(engine_p);
	_mobSelectionUI = new MobSelectionUI(engine_p);
	_towerSelectionUI = new TowerSelectionUI(engine_p);
	_inventoryUI = new InventoryUI(engine_p);

	_buildTowerButton.text = "build tower";
	_buildTowerButton.listener = new SwitchToTowerBuild(engine_p);

	_removeTowerButton.text = "remove tower";
	_removeTowerButton.listener = new RemoveTowerBuild(engine_p);

	GraphicEngine & graphic_l = engine_p.getCellsEngine()->getGraphic();
	graphic_l.registerMessage(new CustomGuiMessage(init_buildTower, this));
}

void LogicUI::update()
{
	if(_header)
	{
		_header->update();
	}
	if(_mobSelectionUI)
	{
		_mobSelectionUI->update();
	}
	if(_towerSelectionUI)
	{
		_towerSelectionUI->update();
	}
	if(_inventoryUI)
	{
		_inventoryUI->update();
	}
}
