#include "AttackModifier.h"

/// @brief return the skin associated with the modifier
std::string AttackModifier::getSkin() const
{
	switch(getAttackType())
	{
		case AttackType::Direct:
			return "DirectModifier";
		case AttackType::Arc:
			return "ArcModifier";
		case AttackType::Splash:
			return "SplashModifier";
		case AttackType::Line:
			return "LineModifier";
	};
	return "DirectModifier";
}

/// @brief return a text description for the modifier
std::vector<InfoLabel> AttackModifier::getDesc() const
{
	std::vector<InfoLabel> content_l;

	return content_l;
}
