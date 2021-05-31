#pragma once

#include "logic/types/ArmorType.h"

#include <array>
#include <string>

struct MobModel
{
	double const hitpoint;
	double const speed;
	ArmorType const armor_type;
	std::string resource;
	std::array<double, 2> const size;
};

struct MobModelLayout
{
	MobModel const model;
	unsigned long const quantity;
	double const delay;
};
