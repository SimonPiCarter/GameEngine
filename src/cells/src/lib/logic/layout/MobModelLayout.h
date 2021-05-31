#pragma once

#include "logic/types/ArmorType.h"

struct MobModel
{
	double const hitpoint;
	double const speed;
	ArmorType const armor_type;
	std::string resource;
};

struct MobModelLayout
{
	MobModel const model;
	unsigned long const quantity;
	double const delay;
};
