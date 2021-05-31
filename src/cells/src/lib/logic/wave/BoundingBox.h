#pragma once

#include <array>
#include <iostream>

struct BoundingBox
{
	std::array<double, 2> position;
	std::array<double, 2> size;
};

/// @brief compute intersection of boxes
BoundingBox intersection(BoundingBox const &box1_p, BoundingBox const &box2_p);

/// @brief check if bounding box is empty (any size <= 0)
bool empty(BoundingBox const &box_p);

/// @brief check if other_p was oversizing ref on the left (pos)
bool oversizeLeft(BoundingBox const &ref_p, BoundingBox const &other_p);

/// @brief check if other_p was oversizing ref on the left (pos + size)
bool oversizeRight(BoundingBox const &ref_p, BoundingBox const &other_p);

/// @brief check if other_p was oversizing ref on the left (pos)
bool oversizeTop(BoundingBox const &ref_p, BoundingBox const &other_p);

/// @brief check if other_p was oversizing ref on the left (pos + size)
bool oversizeBottom(BoundingBox const &ref_p, BoundingBox const &other_p);

std::ostream &operator<<(std::ostream &os_p, BoundingBox const &box_p);
