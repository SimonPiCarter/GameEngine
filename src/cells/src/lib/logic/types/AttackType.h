#pragma once

#include <string>
#include <stdexcept>

enum class AttackType
{
	Direct,
	Arc,
	Splash,
	Line
};

inline std::string to_string(AttackType type_p)
{
	switch(type_p)
	{
		case AttackType::Direct:
			return "Direct";
		case AttackType::Arc:
			return "Arc";
		case AttackType::Splash:
			return "Splash";
		case AttackType::Line:
			return "Line";
	};
	throw std::logic_error("missing implem for AttackType");
}
