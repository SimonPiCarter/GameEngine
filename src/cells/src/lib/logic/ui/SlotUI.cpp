#include "SlotUI.h"

#include "ColibriGui/ColibriButton.h"
#include "ColibriGui/ColibriLabel.h"
#include "ColibriGui/ColibriWindow.h"

#include "logic/slot/Slot.h"

SlotUI * createSlotUI(Colibri::ColibriManager *manager_p, Colibri::Window *window_p, std::array<double, 2> pos_p, unsigned long size_p)
{
	SlotUI * newSlot_l = new SlotUI();
	newSlot_l->_button = manager_p->createWidget<Colibri::Button>(window_p);
	newSlot_l->_button->setTransform( Ogre::Vector2(pos_p[0], pos_p[1]), Ogre::Vector2( size_p, size_p ) );
	float borders_l[4] = {0, 0, 0, 0};
	newSlot_l->_button->setBorderSize(borders_l);
	newSlot_l->_button->setVisualsEnabled(false);

	newSlot_l->_image = manager_p->createWidget<Colibri::Renderable>(newSlot_l->_button);
	newSlot_l->_image->setTransform(Ogre::Vector2(0, 0), Ogre::Vector2(size_p, size_p));

	newSlot_l->_label = manager_p->createWidget<Colibri::Label>(newSlot_l->_button);
	newSlot_l->_label->setShadowOutline(true);
	newSlot_l->_label->setDefaultFontSize(10.f);

	newSlot_l->_select = manager_p->createWidget<Colibri::Renderable>(newSlot_l->_button);
	newSlot_l->_select->setTransform(Ogre::Vector2(0, 0), Ogre::Vector2(size_p, size_p));
	newSlot_l->_select->setSkin("Select");
	newSlot_l->_select->setHidden(true);

	newSlot_l->_disabled = manager_p->createWidget<Colibri::Renderable>(newSlot_l->_button);
	newSlot_l->_disabled->setTransform(Ogre::Vector2(0, 0), Ogre::Vector2(size_p, size_p));
	newSlot_l->_disabled->setSkin("Disabled");
	newSlot_l->_disabled->setHidden(true);
	newSlot_l->_disabled->setZOrder(5);

	newSlot_l->_highlighted = manager_p->createWidget<Colibri::Renderable>(newSlot_l->_button);
	newSlot_l->_highlighted->setTransform(Ogre::Vector2(0, 0), Ogre::Vector2(size_p, size_p));
	newSlot_l->_highlighted->setSkin("Highlighted");
	newSlot_l->_highlighted->setHidden(true);
	newSlot_l->_highlighted->setZOrder(5);

	return newSlot_l;
}

UpdateSlotToolkit::UpdateSlotToolkit(SlotUI &slotUI_p, Slot * slot_p)
	: _slotUI(slotUI_p)
	, _newSlot(slot_p)
{
	_slotUI._slot = slot_p;
	_slotUI._image->setSkin(getSkin(_newSlot));
}

UpdateSlotToolkit::~UpdateSlotToolkit() {}

void UpdateSlotToolkit::update()
{
	long buttonSize_l = _slotUI._button->getSize().x;
	_slotUI._label->setText(getLvl(_newSlot));
	_slotUI._label->sizeToFit();
	std::array<double, 3> col_l = getLvlColour(_newSlot, true);
	_slotUI._label->setTextColour(Ogre::ColourValue(col_l[0], col_l[1], col_l[2], 1.), -1, Colibri::States::NumStates);
	_slotUI._label->setTransform(Ogre::Vector2(buttonSize_l-_slotUI._label->getSize().x-7, buttonSize_l-_slotUI._label->getSize().y),
		_slotUI._label->getSize());
}

void updateSlotUIFromToolkit(CustomGuiToolkit* toolkit_p, GraphicEngine *)
{
	static_cast<UpdateSlotToolkit *>(toolkit_p)->update();
}
