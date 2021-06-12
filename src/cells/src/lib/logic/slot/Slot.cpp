#include "Slot.h"

#include "logic/utils/Translator.h"

Slot::Slot(std::string const &id_p) : _id(id_p) {}
Slot::~Slot() {}

EffectMaker::EffectMaker(std::string const &id_p) : Slot(id_p) {}
EffectMaker::~EffectMaker() {}

/// @brief return true iif Slot is of type EffectMaker
bool EffectMaker::isEffectMaker() const { return true; }
/// @brief return true iif Slot is of type Buff
bool EffectMaker::isBuffMaker() const { return false; }

/// @brief skin getter for a slot that handles empty slot
std::string getSkin(Slot * slot_p)
{
	if(slot_p)
	{
		return slot_p->getSkin();
	}
	return "EmptySlot";
}

/// @brief content getter for a slot that handles empty slot
std::vector<InfoLabel> getDesc(Slot * slot_p)
{
	if(slot_p)
	{
		return slot_p->getDesc();
	}
	std::vector<InfoLabel> content_l;

	content_l.push_back({translate("Empty slot."), 255, 255, 255});

	return content_l;
}
