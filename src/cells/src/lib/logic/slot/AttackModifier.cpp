#include "AttackModifier.h"

#include "logic/utils/Translator.h"

#include <sstream>

/// @brief return the skin associated with the modifier
std::string AttackModifier::getSkin() const
{
	return to_string(getAttackType());
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

/// @brief return a text description for the modifier
std::vector<InfoLabel> AttackModifier::getDesc() const
{
	std::vector<InfoLabel> content_l;

	std::string typeStr_l = to_string(getAttackType())+"Modifier";

	switch(getAttackType())
	{
		case AttackType::Direct:
			content_l.push_back({typeStr_l + "\n", 255, 150, 75});
			content_l.push_back({translate("Deals damage to an enemy.") + "\n", 255, 255, 255});
			break;
		case AttackType::Arc:
			content_l.push_back({typeStr_l + "\n", 150, 150, 255});
			content_l.push_back({translate("Damage jump to the closest enemy.") + "\n", 255, 255, 255});
			break;
		case AttackType::Splash:
			content_l.push_back({typeStr_l + "\n", 150, 20, 20});
			content_l.push_back({translate("Deals damage in an area.") + "\n", 255, 255, 255});
			break;
		case AttackType::Line:
			content_l.push_back({typeStr_l + "\n", 100, 255, 100});
			content_l.push_back({translate("Deals damage to enemies in a line.") + "\n", 255, 255, 255});
			break;
	};

	content_l.push_back({translate("attack speed : ") + to_str(getSpeed())+"\n", 255, 255, 255});
	content_l.push_back({translate("damage         : ") + to_str(getDamage())+"\n", 255, 255, 255});
	content_l.push_back({translate("range            : ") + to_str(getRange())+"\n", 255, 255, 255});

	switch(getAttackType())
	{
		case AttackType::Direct:
			break;
		case AttackType::Arc:
			content_l.push_back({translate("jumps           : ") + to_str(getParam())+"\n", 255, 255, 255});
			break;
		case AttackType::Splash:
			content_l.push_back({translate("area             : ") + to_str(getParam())+"\n", 255, 255, 255});
			break;
		case AttackType::Line:
			break;
	};

	return content_l;
}
