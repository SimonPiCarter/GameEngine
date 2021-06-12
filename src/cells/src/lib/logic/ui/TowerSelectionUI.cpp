#include "TowerSelectionUI.h"

#include <sstream>

#include "game/CellsEngine.h"
#include "logic/utils/Translator.h"
#include "logic/LogicEngine.h"
#include "logic/entity/Tower.h"
#include "logic/slot/Slot.h"

#include "engine/GraphicEngine.h"
#include "gui/ButtonData.h"


TowerSelectionUI::TowerSelectionUI(LogicEngine &engine_p)
	: _engine(engine_p)
	, _currentSelection(nullptr)
{
	GraphicEngine & graphic_l = _engine.getCellsEngine()->getGraphic();
	_pos = {
		graphic_l.getColibriManager()->getCanvasSize().x-500,
		graphic_l.getColibriManager()->getCanvasSize().y-282
	};

	graphic_l.registerMessage(new CustomGuiMessage(init, this));

}

TowerSelectionUI::~TowerSelectionUI()
{
	GraphicEngine & graphic_l = _engine.getCellsEngine()->getGraphic();
	graphic_l.registerMessage(new DestroyWindowMessage(_title));
	graphic_l.registerMessage(new DestroyWindowMessage(_stats.label->getWindow()));
	graphic_l.registerMessage(new DestroyWindowMessage(_slots));
	graphic_l.registerMessage(new DestroyWindowMessage(_windowModifier));
	for(RichLabelVessel * label_l : _tooltipsLabel)
	{
		graphic_l.registerMessage(new DestroyWindowMessage(label_l->label->getWindow()));
		delete label_l;
	}
}

void TowerSelectionUI::setUpTitle()
{
	// Adjust window pos and size
	_title->setTransform( Ogre::Vector2(_pos[0], _pos[1]), Ogre::Vector2( 100, 20 ) );
	_title->m_breadthFirst = true;
	float borders_l[4] = {0, 0, 0, 0};
	_title->setBorderSize(borders_l);

	Colibri::LayoutLine *layout_l = new Colibri::LayoutLine(_manager);

	Colibri::Label * label_l = _manager->createWidget<Colibri::Label>(_title);
	label_l->m_minSize = Ogre::Vector2( 350, 64 );
	label_l->setText("Tower");
	label_l->setTextHorizAlignment(Colibri::TextHorizAlignment::Center);
	label_l->setTextVertAlignment(Colibri::TextVertAlignment::Center);
	layout_l->addCell(label_l);

	layout_l->setAdjustableWindow(_title);
	layout_l->m_hardMaxSize = _manager->getCanvasSize();
	layout_l->layout();

	// no scroll here!
	_title->setMaxScroll(Ogre::Vector2(0,0));
	_title->update(0.f);

	label_l->updateDerivedTransformFromParent(true);
	_title->updateDerivedTransformFromParent(true);
}

void TowerSelectionUI::setUpStats()
{
	std::vector<InfoLabel> content_l;
	content_l.push_back({"Stats", 255, 255, 0});
	content_l.push_back({" : 12\n", 255, 255, 255});

	long sizeModifier_l = 130;
	GraphicEngine & graphic_l = _engine.getCellsEngine()->getGraphic();
	_stats.label = new RichLabel(content_l, _pos[0], _pos[1] + 50, 350, 3*69 + 25 - sizeModifier_l, 10, true, graphic_l);
	_stats.label->setHidden(true);

	// Adjust window pos and size
	_windowModifier = _manager->createWindow(nullptr);
	_windowModifier->setTransform( Ogre::Vector2(_pos[0], _pos[1] + 50 + 3*69+25- sizeModifier_l), Ogre::Vector2( 350, sizeModifier_l ) );
	_windowModifier->m_breadthFirst = true;
	_windowModifier->m_minSize = Ogre::Vector2( 350, sizeModifier_l );
	float borders_l[4] = {0, 0, 0, 0};
	_windowModifier->setBorderSize(borders_l);

	_modifier = _manager->createWidget<Colibri::Button>(_windowModifier);
	_modifier->setTransform( Ogre::Vector2(350/2-sizeModifier_l/2, 0), Ogre::Vector2(sizeModifier_l, sizeModifier_l) );

	_tooltipsLabel.push_back(new RichLabelVessel());
	_tooltipsLabel.back()->label = new RichLabel(content_l, _pos[0] + 5, _pos[1], 490, 0., 10, true, graphic_l);
	_tooltipsLabel.back()->label->setBottomAnchor(_pos[1] - 5.);
	_tooltipsLabel.back()->label->setHidden(true);
	_listeners.push_back(new TooltipListener(graphic_l.getColibriManager(), _modifier, _tooltipsLabel.back()->label, false));
	_modifier->addActionListener(_listeners.back());

	// no scroll here!
	_windowModifier->setMaxScroll(Ogre::Vector2(0,0));
	_windowModifier->updateDerivedTransformFromParent(true);
}

void TowerSelectionUI::setUpSlots()
{
	// Adjust window pos and size
	_slots->setTransform( Ogre::Vector2(_pos[0]+350, _pos[1] + 50), Ogre::Vector2( 150, 3*69 + 20 ) );
	_slots->m_breadthFirst = true;
	float borders_l[4] = {0, 0, 0, 0};
	_slots->setBorderSize(borders_l);

	_layout = new Colibri::LayoutLine(_manager);

	_layout->setAdjustableWindow(_slots);
	_layout->m_hardMaxSize = 3*69 + 45;
	_layout->layout();

	_slots->setMaxScroll(Ogre::Vector2(0,0));
	_slots->updateDerivedTransformFromParent(true);
}

void init(CustomGuiToolkit * toolkit_p, GraphicEngine * graphic_p)
{
	TowerSelectionUI * ui_l = static_cast<TowerSelectionUI *>(toolkit_p);
	ui_l->_manager = graphic_p->getColibriManager();

	ui_l->_title = ui_l->_manager->createWindow(nullptr);
	ui_l->setUpTitle();

	ui_l->setUpStats();

	ui_l->_slots = ui_l->_manager->createWindow(nullptr);
	ui_l->setUpSlots();

	hide(toolkit_p, graphic_p);
}

void hide(CustomGuiToolkit * toolkit_p, GraphicEngine *)
{
	TowerSelectionUI * ui_l = static_cast<TowerSelectionUI *>(toolkit_p);
	ui_l->_title->setHidden(true);
	ui_l->_title->setClickable(false);
	ui_l->_stats.label->setHidden(true);
	ui_l->_windowModifier->setHidden(true);
	ui_l->_windowModifier->setClickable(false);
	ui_l->_slots->setHidden(true);
	ui_l->_slots->setClickable(false);
	for(RichLabelVessel * tooltips_l : ui_l->_tooltipsLabel)
	{
		tooltips_l->label->setHidden(true);
	}
}

void show(CustomGuiToolkit * toolkit_p, GraphicEngine *)
{
	TowerSelectionUI * ui_l = static_cast<TowerSelectionUI *>(toolkit_p);
	ui_l->_title->setHidden(false);
	ui_l->_title->setClickable(true);
	ui_l->_stats.label->setHidden(false);
	ui_l->_windowModifier->setHidden(false);
	ui_l->_windowModifier->setClickable(true);
	ui_l->_slots->setHidden(false);
	ui_l->_slots->setClickable(true);
}

void slot(CustomGuiToolkit * toolkit_p, GraphicEngine *)
{
	TowerSelectionUI * ui_l = static_cast<TowerSelectionUI *>(toolkit_p);
	ui_l->updateSlots();
}

void TowerSelectionUI::update()
{
	GraphicEngine & graphic_l = _engine.getCellsEngine()->getGraphic();

	if(_engine.getTowerSelection())
	{
		if(_currentSelection != _engine.getTowerSelection())
		{
			// update UI from selection
			_currentSelection = _engine.getTowerSelection();
			updateSelection(_currentSelection);
		}
		graphic_l.registerMessage(new CustomGuiMessage(&show, this));
	}
	else
	{
		graphic_l.registerMessage(new CustomGuiMessage(&hide, this));
	}
}

namespace
{
	std::string to_str(double d)
	{
		std::stringstream ss_l;
		ss_l<<d;
		return ss_l.str();
	}
}

void TowerSelectionUI::updateSelection(Tower * tower_p)
{
	GraphicEngine & graphic_l = _engine.getCellsEngine()->getGraphic();

	// update stats
	std::vector<InfoLabel> content_l;

	content_l.push_back({"Damage        : ", 255, 255, 255});
	content_l.push_back({to_str(tower_p->getDamage())+"\n", 255, 255, 0});
	content_l.push_back({"AttackSpeed : ", 255, 255, 255});
	content_l.push_back({to_str(tower_p->getAttackSpeed())+"\n", 255, 255, 0});
	content_l.push_back({"Range           : ", 255, 255, 255});
	content_l.push_back({to_str(tower_p->getRange())+"\n", 255, 255, 0});

	graphic_l.registerMessage(new UpdateTextRichLabelMessage(_stats, content_l));

	// update attack modifier tooltip and icon
	_modifier->setSkin(tower_p->getAttackModifier().getSkin()+"Modifier");
	graphic_l.registerMessage(new UpdateTextRichLabelMessage(*_tooltipsLabel[0], tower_p->getAttackModifier().getDesc()));

	graphic_l.registerMessage(new CustomGuiMessage(&slot, this));
}

void TowerSelectionUI::updateSlots()
{
	GraphicEngine & graphic_l = _engine.getCellsEngine()->getGraphic();
	size_t slots_number_l = _currentSelection->getMaxSlots();
	// create new buttons if necessary
	for(size_t i = _buttons.size() ; i < slots_number_l ; ++ i)
	{
		// create button
		_buttons.push_back(_manager->createWidget<Colibri::Button>(_slots));
		size_t buttonSize_l = 75;
		_buttons.back()->m_minSize = Ogre::Vector2( buttonSize_l, buttonSize_l );
		_layout->addCell(_buttons.back());

		std::vector<InfoLabel> content_l;
		content_l.push_back({"empty.", 255, 255, 255});

		// create tooltip
		_tooltipsLabel.push_back(new RichLabelVessel());
		_tooltipsLabel.back()->label = new RichLabel(content_l, _pos[0] + 5, _pos[1], 490, 0., 10, true, graphic_l);
		_tooltipsLabel.back()->label->setBottomAnchor(_pos[1] - 5.);
		_tooltipsLabel.back()->label->setHidden(true);
		_listeners.push_back(new TooltipListener(graphic_l.getColibriManager(), _buttons.back(), _tooltipsLabel.back()->label, false));
		_buttons.back()->addActionListener(_listeners.back());
		_buttons.back()->updateDerivedTransformFromParent(true);
		_tooltipsLabel.back()->label->getWindow()->updateDerivedTransformFromParent(true);
	}

	// Show only necesssary slots buttons
	for(size_t i = 0 ; i < _buttons.size() ; ++ i)
	{
		// update image from slot
		if(i < slots_number_l)
		{
			_buttons[i]->setHidden(false);
			_buttons[i]->setSkin(getSkin(_currentSelection->getSlots()[i]));
			// update tooltip
			// index is shifted by 1 because first tooltip is attack modifier
			graphic_l.registerMessage(new UpdateTextRichLabelMessage(*_tooltipsLabel[i+1], getDesc(_currentSelection->getSlots()[i])));
		}
		else
		{
			_buttons[i]->setHidden(true);
		}
	}

	_layout->layout();

	double size_l = 0.;
	if(!_buttons.empty())
	{
		size_l = _buttons.back()->getSize().y;
	}

	_slots->setMaxScroll(Ogre::Vector2(0,std::max(0.,(slots_number_l-3)*size_l)));
}
