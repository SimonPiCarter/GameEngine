#include "SlotUI.h"

#include "ColibriGui/ColibriButton.h"
#include "ColibriGui/ColibriLabel.h"
#include "ColibriGui/ColibriWindow.h"

#include "logic/slot/Slot.h"

SlotUI * createSlotUI(Colibri::ColibriManager *manager_p, Colibri::Window *window_p, std::array<double, 2> pos_p, unsigned long size_p)
{
	SlotUI * newSlot_l = new SlotUI();
	newSlot_l->_button = manager_p->createWidget<Colibri::Button>(window_p);
	newSlot_l->_button->setTransform( Ogre::Vector2(pos_p[0]-100, pos_p[1]-100), Ogre::Vector2( size_p, size_p ) );
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
	return newSlot_l;
}

void updateSlotUI(SlotUI &slotUI_p, Slot * slot_p)
{
	slotUI_p._slot = slot_p;
	long buttonSize_l = slotUI_p._button->getSize().x;
	slotUI_p._image->setSkin(getSkin(slot_p));
	slotUI_p._label->setText(getLvl(slot_p));
	slotUI_p._label->sizeToFit();
	std::array<double, 3> col_l = getLvlColour(slot_p, true);
	slotUI_p._label->setTextColour(Ogre::ColourValue(col_l[0], col_l[1], col_l[2], 1.), -1, Colibri::States::NumStates);
	slotUI_p._label->setTransform(Ogre::Vector2(buttonSize_l-slotUI_p._label->getSize().x-7, buttonSize_l-slotUI_p._label->getSize().y),
		slotUI_p._label->getSize());
}
