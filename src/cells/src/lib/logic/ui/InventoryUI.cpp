#include "InventoryUI.h"

#include <sstream>

#include "game/CellsEngine.h"
#include "logic/utils/Translator.h"
#include "logic/LogicEngine.h"
#include "logic/entity/Tower.h"
#include "logic/slot/Slot.h"

#include "engine/GraphicEngine.h"
#include "gui/ButtonData.h"


InventoryUI::InventoryUI(LogicEngine &engine_p)
	: _engine(engine_p)
	, _towerModifierSlot(nullptr)
	, _currentSelectedSlot(nullptr)
	, _pos({300 , 300})
	, _size({720 , 720})
	, _posTower({1025, 660})
	, _sizeTower({120, 360})
	, _posButton({300, 1025})
	, _sizeButton({720, 100})
	, _lines(6)
	, _slotSize(120)
	, _cancel(false)
	, _reset(false)
	, _save(false)
{
	GraphicEngine & graphic_l = _engine.getCellsEngine()->getGraphic();
	graphic_l.registerMessage(new CustomGuiMessage(init_inventory, this));
}

InventoryUI::~InventoryUI()
{
	GraphicEngine & graphic_l = _engine.getCellsEngine()->getGraphic();
	graphic_l.registerMessage(new DestroyWindowMessage(_main));
	graphic_l.registerMessage(new DestroyWindowMessage(_towerWindow));
	graphic_l.registerMessage(new DestroyWindowMessage(_towerModifierWindow));
	graphic_l.registerMessage(new DestroyWindowMessage(_buttonWindow));
	for(RichLabelVessel * label_l : _tooltipsLabel)
	{
		graphic_l.registerMessage(new DestroyWindowMessage(label_l->label->getWindow()));
		delete label_l;
	}
	for(Colibri::WidgetActionListener* listener_l: _listeners)
	{
		delete listener_l;
	}
	for(SlotUI * ui_l : _slots)
	{
		delete ui_l;
	}
	for(SlotUI * ui_l : _towerSlots)
	{
		delete ui_l;
	}
	delete _layout;
	delete _towerLayout;
	delete _buttonLayout;

	delete _towerModifierSlot;
}


void init_inventory(CustomGuiToolkit * toolkit_p, GraphicEngine * graphic_p)
{
	InventoryUI * ui_l = static_cast<InventoryUI *>(toolkit_p);
	ui_l->_manager = graphic_p->getColibriManager();

	ui_l->setUpMain();
	ui_l->setUpTower();
	ui_l->setUpButtons();

	hide_inventory(toolkit_p, graphic_p);
}

void hide_inventory(CustomGuiToolkit * toolkit_p, GraphicEngine *)
{
	InventoryUI * ui_l = static_cast<InventoryUI *>(toolkit_p);
	ui_l->_main->setHidden(true);
	ui_l->_main->setClickable(false);
	for(RichLabelVessel * tooltips_l : ui_l->_tooltipsLabel)
	{
		tooltips_l->label->setHidden(true);
	}
	for(RichLabelVessel * tooltips_l : ui_l->_tooltipsLabelTower)
	{
		tooltips_l->label->setHidden(true);
	}
	ui_l->_towerWindow->setHidden(true);
	ui_l->_towerWindow->setClickable(false);

	ui_l->_towerModifierWindow->setHidden(true);
	ui_l->_towerModifierWindow->setClickable(false);

	ui_l->_buttonWindow->setHidden(true);
	ui_l->_buttonWindow->setClickable(false);

	for(Colibri::Button * button_l : ui_l->_allButtons)
	{
		button_l->setClickable(false);
	}
}

void show_inventory(CustomGuiToolkit * toolkit_p, GraphicEngine *)
{
	InventoryUI * ui_l = static_cast<InventoryUI *>(toolkit_p);
	ui_l->_main->setHidden(false);
	ui_l->_main->setClickable(true);

	ui_l->_towerWindow->setHidden(false);
	ui_l->_towerWindow->setClickable(true);

	ui_l->_towerModifierWindow->setHidden(false);
	ui_l->_towerModifierWindow->setClickable(true);

	ui_l->_buttonWindow->setHidden(false);
	ui_l->_buttonWindow->setClickable(true);

	for(Colibri::Button * button_l : ui_l->_allButtons)
	{
		button_l->setClickable(true);
	}

	// update from inventory
	ui_l->updateInventory();

	// update tower
	ui_l->updateTower();
}

void InventoryUI::update()
{
	if(_reset)
	{
		_engine.setInventoryHidden(true);
		_engine.setInventoryHidden(false);
		_reset = false;
	}
	if(_cancel)
	{
		_engine.setInventoryHidden(true);
		_cancel = false;
	}
	if(_save)
	{
		saveConf();
	}
}

void InventoryUI::setCurrentSlotSelection(SlotUI * selection_p)
{
	if(_currentSelectedSlot)
	{
		_currentSelectedSlot->_select->setHidden(true);
		// update higlighted
		if(_currentSelectedSlot->_slot->isAttackModifier())
		{
			// de-highlight attack modifier
			_towerModifierSlot->_highlighted->setHidden(true);
		}
		else
		{
			// de-highlight slots tower
			for(SlotUI * slot_l : _towerSlots)
			{
				slot_l->_highlighted->setHidden(true);
			}
		}
	}
	_currentSelectedSlot = selection_p;
	if(_currentSelectedSlot)
	{
		_currentSelectedSlot->_select->setHidden(false);
		// update higlighted
		if(_currentSelectedSlot->_slot->isAttackModifier())
		{
			// highlight attack modifier
			_towerModifierSlot->_highlighted->setHidden(_towerModifierSlot->_button->isHidden());
		}
		else
		{
			// highlight slots tower
			for(SlotUI * slot_l : _towerSlots)
			{
				slot_l->_highlighted->setHidden(slot_l->_button->isHidden());
			}
		}
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

void InventoryUI::setUpMain()
{
	GraphicEngine & graphic_l = _engine.getCellsEngine()->getGraphic();
	_main = _manager->createWindow(nullptr);
	_main->setTransform( Ogre::Vector2(_pos[0], _pos[1]), Ogre::Vector2( _size[0], _size[1] ) );
	_main->m_breadthFirst = true;
	float borders_l[4] = {0, 0, 0, 0};
	_main->setBorderSize(borders_l);

	_layout = new Colibri::LayoutMultiline(_manager);
	_layout->m_numLines = _lines;
	_layout->m_vertical = true;

	_layout->setAdjustableWindow(_main);
	_layout->m_hardMaxSize = _size[0];
	_layout->layout();
}

void InventoryUI::setUpTower()
{
	GraphicEngine & graphic_l = _engine.getCellsEngine()->getGraphic();
	_towerWindow = _manager->createWindow(nullptr);
	_towerWindow->setTransform( Ogre::Vector2(_posTower[0], _posTower[1]), Ogre::Vector2( _sizeTower[0], _sizeTower[1] ) );
	_towerWindow->m_breadthFirst = true;
	float borders_l[4] = {0, 0, 0, 0};
	_towerWindow->setBorderSize(borders_l);

	_towerModifierWindow = _manager->createWindow(nullptr);;
	_towerModifierWindow->setTransform( Ogre::Vector2(_posTower[0], _posTower[1]- _slotSize - 5), Ogre::Vector2( _sizeTower[0], _slotSize +5 ) );
	_towerModifierWindow->m_breadthFirst = true;
	_towerModifierWindow->setBorderSize(borders_l);

	_towerModifierSlot = createSlotUI(_manager, _towerModifierWindow, {0, 0}, _slotSize);
	graphic_l.registerMessage(new CustomGuiMessage(updateSlotUIFromToolkit, new UpdateSlotToolkit(*_towerModifierSlot, nullptr), true));

	// create tooltip
	std::vector<InfoLabel> content_l;
	content_l.push_back({"empty.", 255, 255, 255});

	_tooltipsLabelTower.push_back(new RichLabelVessel());
	_tooltipsLabelTower.back()->label = new RichLabel(content_l, _posTower[0] + _sizeTower[0] + 5, _pos[1] + _size[1]/2. + 10., 490, 0., 10, true, graphic_l);
	_tooltipsLabelTower.back()->label->setHidden(true);
	_towerModifierSlot->_tooltip = _tooltipsLabelTower.back();

	_listeners.push_back(new TooltipListener(graphic_l.getColibriManager(), _towerModifierSlot->_button, _tooltipsLabelTower.back()->label, false));
	_towerModifierSlot->_button->addActionListener(_listeners.back());

	_listeners.push_back(new SelectionTowerUIListener(*_towerModifierSlot, *this, _engine));
	_towerModifierSlot->_button->addActionListener(_listeners.back());
	_allButtons.push_back(_towerModifierSlot->_button);

	_towerLayout = new Colibri::LayoutLine(_manager);
	_towerLayout->m_vertical = true;

	_towerLayout->setAdjustableWindow(_towerWindow);
	_towerLayout->m_hardMaxSize = _sizeTower[1];
	_towerLayout->layout();
}

void InventoryUI::setUpButtons()
{
	GraphicEngine & graphic_l = _engine.getCellsEngine()->getGraphic();
	_buttonWindow = _manager->createWindow(nullptr);
	_buttonWindow->setTransform( Ogre::Vector2(_posButton[0], _posButton[1]), Ogre::Vector2( _sizeButton[0], _sizeButton[1] ) );
	_buttonWindow->m_breadthFirst = true;
	float borders_l[4] = {0, 0, 0, 0};
	_buttonWindow->setBorderSize(borders_l);

	_buttonLayout = new Colibri::LayoutLine(_manager);
	_buttonLayout->m_vertical = false;

	_buttonLayout->setAdjustableWindow(_buttonWindow);
	_buttonLayout->m_hardMaxSize = _sizeButton[0];
	_buttonLayout->layout();

	Colibri::Button *button_l = _manager->createWidget<Colibri::Button>(_buttonWindow);
	button_l->m_minSize = Ogre::Vector2( 200, 75 );
	button_l->getLabel()->setText("Cancel");
	button_l->sizeToFit();
	_listeners.push_back(new CancelInventoryListener(button_l, *this));
	button_l->addActionListener(_listeners.back());
	_buttonLayout->addCell(button_l);

	button_l = _manager->createWidget<Colibri::Button>(_buttonWindow);
	button_l->m_minSize = Ogre::Vector2( 200, 75 );
	button_l->getLabel()->setText("Reset");
	button_l->sizeToFit();
	_listeners.push_back(new ResetInventoryListener(button_l, *this));
	button_l->addActionListener(_listeners.back());
	_buttonLayout->addCell(button_l);

	button_l = _manager->createWidget<Colibri::Button>(_buttonWindow);
	button_l->m_minSize = Ogre::Vector2( 200, 75 );
	button_l->getLabel()->setText("Save");
	button_l->sizeToFit();
	_listeners.push_back(new SaveInventoryListener(button_l, *this));
	button_l->addActionListener(_listeners.back());
	_buttonLayout->addCell(button_l);

	_buttonLayout->layout();
}

void InventoryUI::updateInventory()
{
	GraphicEngine & graphic_l = _engine.getCellsEngine()->getGraphic();

	size_t buttonSize_l = _size[0] / _lines;

	size_t slots_number_l = _engine.getInventorySlots().size();

	for(size_t i = _slots.size() ; i < slots_number_l || i%_layout->m_numLines != 0 ; ++ i)
	{
		// create button
		SlotUI * newSlot_l = createSlotUI(_manager, _main, _pos, buttonSize_l);
		_layout->addCell(newSlot_l->_button);

		// create tooltip
		std::vector<InfoLabel> content_l;
		content_l.push_back({"empty.", 255, 255, 255});

		_tooltipsLabel.push_back(new RichLabelVessel());
		_tooltipsLabel.back()->label = new RichLabel(content_l, _posTower[0] + _sizeTower[0] + 5, _pos[1], 490, 0., 10, true, graphic_l);
		_tooltipsLabel.back()->label->setBottomAnchor(_pos[1] + _size[1]/2. - 10.);
		_tooltipsLabel.back()->label->setHidden(true);

		_listeners.push_back(new TooltipListener(graphic_l.getColibriManager(), newSlot_l->_button, _tooltipsLabel.back()->label, false));
		newSlot_l->_button->addActionListener(_listeners.back());

		_listeners.push_back(new SelectionInventoryUIListener(*newSlot_l, *this));
		newSlot_l->_button->addActionListener(_listeners.back());

		_slots.push_back(newSlot_l);
		_allButtons.push_back(newSlot_l->_button);
	}
	// Show only necesssary slots buttons
	for(size_t i = 0 ; i < _slots.size() ; ++ i)
	{
		// update image from slot
		if(i < slots_number_l)
		{
			graphic_l.registerMessage(new CustomGuiMessage(updateSlotUIFromToolkit, new UpdateSlotToolkit(*_slots[i], _engine.getInventorySlots()[i]), true));
			_slots[i]->_button->setHidden(false);
			_slots[i]->_tooltip = _tooltipsLabel[i];

			// update tooltip
			graphic_l.registerMessage(new UpdateTextRichLabelMessage(*_tooltipsLabel[i], getDesc(_engine.getInventorySlots()[i])));
		}
		else
		{
			graphic_l.registerMessage(new CustomGuiMessage(updateSlotUIFromToolkit, new UpdateSlotToolkit(*_slots[i], nullptr), true));
			_slots[i]->_button->setHidden(false);
			_slots[i]->_tooltip = _tooltipsLabel[i];
		}
		// re enable every slot
		_slots[i]->_disabled->setHidden(true);
	}
	_layout->layout();
}

void InventoryUI::updateTower()
{
	GraphicEngine & graphic_l = _engine.getCellsEngine()->getGraphic();

	size_t buttonSize_l = _slotSize;

	Tower * towerSelection_l = _engine.getTowerSelection();

	size_t slots_number_l = 0;
	if(towerSelection_l)
	{
		slots_number_l = towerSelection_l->getMaxSlots();
	}

	for(size_t i = _towerSlots.size() ; i < slots_number_l  ; ++ i)
	{
		// create button
		SlotUI * newSlot_l = createSlotUI(_manager, _towerWindow, _posTower, buttonSize_l);
		_towerLayout->addCell(newSlot_l->_button);

		// create tooltip
		std::vector<InfoLabel> content_l;
		content_l.push_back({"empty.", 255, 255, 255});

		_tooltipsLabelTower.push_back(new RichLabelVessel());
		_tooltipsLabelTower.back()->label = new RichLabel(content_l, _posTower[0] + _sizeTower[0] + 5, _pos[1] + _size[1]/2. + 10., 490, 0., 10, true, graphic_l);
		_tooltipsLabelTower.back()->label->setHidden(true);

		_listeners.push_back(new TooltipListener(graphic_l.getColibriManager(), newSlot_l->_button, _tooltipsLabelTower.back()->label, false));
		newSlot_l->_button->addActionListener(_listeners.back());

		_listeners.push_back(new SelectionTowerUIListener(*newSlot_l, *this, _engine));
		newSlot_l->_button->addActionListener(_listeners.back());

		_towerSlots.push_back(newSlot_l);
		_allButtons.push_back(newSlot_l->_button);
	}

	// reset all origin slots
	for(auto * listener_l : _listeners)
	{
		SelectionTowerUIListener * selectTower_l = dynamic_cast<SelectionTowerUIListener *>(listener_l);
		if(selectTower_l)
		{
			selectTower_l->resetOriginSlot();
		}
	}

	// Show only necesssary slots buttons
	for(size_t i = 0 ; i < _towerSlots.size() ; ++ i)
	{
		// update image from slot
		if(i < slots_number_l)
		{
			graphic_l.registerMessage(new CustomGuiMessage(updateSlotUIFromToolkit, new UpdateSlotToolkit(*_towerSlots[i], towerSelection_l->getSlots().at(i)), true));
			_towerSlots[i]->_button->setHidden(false);
			_towerSlots[i]->_tooltip = _tooltipsLabelTower[i+1];

			// update tooltip
			graphic_l.registerMessage(new UpdateTextRichLabelMessage(*_tooltipsLabelTower[i+1], getDesc(towerSelection_l->getSlots().at(i))));

		}
		else
		{
			_towerSlots[i]->_button->setHidden(true);
		}
	}
	if(towerSelection_l)
	{
		graphic_l.registerMessage(new CustomGuiMessage(updateSlotUIFromToolkit, new UpdateSlotToolkit(*_towerModifierSlot, &towerSelection_l->getAttackModifier()), true));
		graphic_l.registerMessage(new UpdateTextRichLabelMessage(*_tooltipsLabelTower[0], getDesc(&towerSelection_l->getAttackModifier())));
		_towerModifierSlot->_button->setHidden(false);
		_towerModifierSlot->_button->setClickable(true);
	}
	else
	{
		_towerModifierSlot->_button->setHidden(true);
		_towerModifierSlot->_button->setClickable(false);
	}


	_towerLayout->layout();
}

void InventoryUI::saveConf()
{
	Tower * tower_l = _engine.getTowerSelection();
	if(tower_l)
	{
		// set of removed slot from the tower
		std::set<Slot *> toBeRemovedSlots_l;
		// set of consumed invetory slots
		std::set<Slot *> consumedSlots_l;

		// update slots of the tower
		for(unsigned long i = 0 ; i < tower_l->getMaxSlots() ; ++ i)
		{
			if(tower_l->getSlots()[i] != getTowerSlots()[i]->_slot)
			{
				if(tower_l->getSlots()[i])
				{
					toBeRemovedSlots_l.insert(tower_l->getSlots()[i]);
				}
				if(getTowerSlots()[i]->_slot)
				{
					consumedSlots_l.insert(getTowerSlots()[i]->_slot);
				}
			}
			tower_l->getSlots()[i] = getTowerSlots()[i]->_slot;
		}

		if(dynamic_cast<AttackModifier *>(_towerModifierSlot->_slot))
		{
			AttackModifier * toDelete_l = new AttackModifier(tower_l->getAttackModifier());
			consumedSlots_l.insert(_towerModifierSlot->_slot);
			tower_l->setAttackModifier(*dynamic_cast<AttackModifier *>(_towerModifierSlot->_slot));
			toBeRemovedSlots_l.insert(toDelete_l);
		}

		//// logic update
		// update inventory
		_engine.updateInventory(consumedSlots_l);
		// delete slots
		_engine.deleteSlots(toBeRemovedSlots_l);

		// close inventory
		_engine.setInventoryHidden(true);

		_engine.updateTowerSelection();
	}
	_save = false;
}

SelectionInventoryUIListener::SelectionInventoryUIListener(SlotUI &slot_p, InventoryUI &ui_p)
	: _slot(slot_p), _ui(ui_p)
{}
SelectionInventoryUIListener::~SelectionInventoryUIListener() {}

void SelectionInventoryUIListener::notifyWidgetAction( Colibri::Widget *widget, Colibri::Action::Action action )
{
	if(widget != _slot._button || !_slot._slot || !_slot._disabled->isHidden())
	{
		return;
	}
	if(action == Colibri::Action::Action::PrimaryActionPerform)
	{
		// update selection
		_ui.setCurrentSlotSelection(&_slot);
	}
}

SelectionTowerUIListener::SelectionTowerUIListener(SlotUI &slot_p, InventoryUI &ui_p, LogicEngine &engine_p)
	: _slot(slot_p), _originSlot(nullptr), _ui(ui_p), _engine(engine_p)
{}
SelectionTowerUIListener::~SelectionTowerUIListener() {}

void SelectionTowerUIListener::notifyWidgetAction( Colibri::Widget *widget, Colibri::Action::Action action )
{
	if(widget != _slot._button)
	{
		return;
	}
	if(action == Colibri::Action::Action::PrimaryActionPerform)
	{
		// if current selection and slot is highlighted for modification
		if(_ui.getCurrentSlotSelection()
		&& !_slot._highlighted->isHidden())
		{
			GraphicEngine & graphic_l = _engine.getCellsEngine()->getGraphic();
			// replace slot in tower
			graphic_l.registerMessage(new CustomGuiMessage(updateSlotUIFromToolkit, new UpdateSlotToolkit(_slot, _ui.getCurrentSlotSelection()->_slot), true));
			// update tooltip
			graphic_l.registerMessage(new UpdateTextRichLabelMessage(*_slot._tooltip, getDesc(_slot._slot)));

			// if origin slot we re enable it
			if(_originSlot)
			{
				_originSlot->_disabled->setHidden(true);
			}
			// set origin slot from inventory
			_originSlot = _ui.getCurrentSlotSelection();
			// remove select marker
			_ui.getCurrentSlotSelection()->_disabled->setHidden(false);
			_ui.setCurrentSlotSelection(nullptr);
		}
	}
}


SaveInventoryListener::SaveInventoryListener(Colibri::Button *button_p, InventoryUI &ui_p)
	: _button(button_p), _ui(ui_p)
{}
SaveInventoryListener::~SaveInventoryListener() {}

void SaveInventoryListener::notifyWidgetAction( Colibri::Widget *widget, Colibri::Action::Action action )
{
	if(widget != _button)
	{
		return;
	}
	if(action == Colibri::Action::Action::PrimaryActionPerform)
	{
		_ui.triggerSave();
	}
}

ResetInventoryListener::ResetInventoryListener(Colibri::Button *button_p, InventoryUI &ui_p)
	: _button(button_p), _ui(ui_p)
{}
ResetInventoryListener::~ResetInventoryListener() {}

void ResetInventoryListener::notifyWidgetAction( Colibri::Widget *widget, Colibri::Action::Action action )
{
	if(widget != _button)
	{
		return;
	}
	if(action == Colibri::Action::Action::PrimaryActionPerform)
	{
		_ui.triggerReset();
	}
}

CancelInventoryListener::CancelInventoryListener(Colibri::Button *button_p, InventoryUI &ui_p)
	: _button(button_p), _ui(ui_p)
{}
CancelInventoryListener::~CancelInventoryListener() {}

void CancelInventoryListener::notifyWidgetAction( Colibri::Widget *widget, Colibri::Action::Action action )
{
	if(widget != _button)
	{
		return;
	}
	if(action == Colibri::Action::Action::PrimaryActionPerform)
	{
		_ui.triggerCancel();
	}
}
