#pragma once

#include <array>

struct Hitbox
{
	Hitbox()
		: offset({0,0,0}), size({1,1,1})
	{}
	Hitbox(std::array<double, 3> offset_p, std::array<double, 3> size_p)
		: offset(offset_p), size(size_p)
	{}

	std::array<double, 3> offset;
	std::array<double, 3> size;
};

struct PositionalHitbox
{
	Hitbox hitbox;
	std::array<double, 3> pos;
};

/// @brief check if a ray (orig_p -dir_p-> ) collide with the positioned hitbox
/// @param orig_p the origin of the ray
/// @param dir_p the direction of the ray
/// @param hitbox_p the hitbox to be tested
/// @return return the distance (in norm of dir_p!), negative if no contact
double collide(std::array<double, 3> const &orig_p, std::array<double, 3> const &dir_p, PositionalHitbox const &hitbox_p);
