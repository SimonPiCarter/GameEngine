#include "Slot.h"

#include "logic/utils/Translator.h"

Slot::Slot(std::string const &id_p, unsigned long lvl_p) : _id(id_p), _lvl(lvl_p) {}
Slot::~Slot() {}

EffectMaker::EffectMaker(std::string const &id_p, unsigned long lvl_p) : Slot(id_p, lvl_p) {}
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
		std::vector<InfoLabel> desc_l = slot_p->getDesc();
		desc_l.push_back({translate("\nlevel : "), 255, 255, 255});
		std::array<double, 3> col_l = getLvlColour(slot_p);
		desc_l.push_back({getLvl(slot_p), col_l[0], col_l[1], col_l[2]});
		return desc_l;
	}
	std::vector<InfoLabel> content_l;

	content_l.push_back({translate("Empty slot."), 255, 255, 255});

	return content_l;
}

std::string getLvl(Slot * slot_p)
{
	if(slot_p)
	{
		return std::to_string(slot_p->getLvl());
	}
	return "";
}

std::array<double, 3> getLvlColour(Slot * slot_p)
{
	if(slot_p)
	{
		if(slot_p->getLvl() < 25)
		{
			return {255.,255.,255.};
		}
		if(slot_p->getLvl() < 50)
		{
			return {0.,255.,0.};
		}
		if(slot_p->getLvl() < 75)
		{
			return {0.,0.,255.};
		}
		if(slot_p->getLvl() < 100)
		{
			return {155.,0.,0.};
		}
		if(slot_p->getLvl() < 150)
		{
			return {75.,0.,75.};
		}
		if(slot_p->getLvl() < 200)
		{
			return {75.,75.,0.};
		}
		return {0.,75.,75.};
	}
	return {0.,0.,0.};
}
