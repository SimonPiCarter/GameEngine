#include "BoundingBox.h"


/// @brief compute intersection of boxes
BoundingBox intersection(BoundingBox const &box1_p, BoundingBox const &box2_p)
{
	BoundingBox box_l = {
			{std::max(box1_p.position[0], box2_p.position[0]), std::max(box1_p.position[1], box2_p.position[1])},
			{std::min(box1_p.position[0] + box1_p.size[0], box2_p.position[0] + box2_p.size[0])
				, std::min(box1_p.position[1]  + box1_p.size[1], box2_p.position[1]+ + box2_p.size[1])}
		};
	box_l.size[0] = box_l.size[0] - box_l.position[0];
	box_l.size[1] = box_l.size[1] - box_l.position[1];
	return box_l;
}

/// @brief check if bounding box is empty (any size <= 0)
bool empty(BoundingBox const &box_p)
{
	return box_p.size[0] <= 0. || box_p.size[1] <= 0.;
}

/// @brief check if other_p was oversizing ref on the left (pos)
bool oversizeLeft(BoundingBox const &ref_p, BoundingBox const &other_p)
{
	return ref_p.position[0] > other_p.position[0];
}

/// @brief check if other_p was oversizing ref on the left (pos + size)
bool oversizeRight(BoundingBox const &ref_p, BoundingBox const &other_p)
{
	return ref_p.position[0] + ref_p.size[0] < other_p.position[0] + other_p.size[0];
}

/// @brief check if other_p was oversizing ref on the left (pos)
bool oversizeTop(BoundingBox const &ref_p, BoundingBox const &other_p)
{
	return ref_p.position[1] > other_p.position[1];
}

/// @brief check if other_p was oversizing ref on the left (pos + size)
bool oversizeBottom(BoundingBox const &ref_p, BoundingBox const &other_p)
{
	return ref_p.position[1] + ref_p.size[1] < other_p.position[1] + other_p.size[1];
}

std::ostream &operator<<(std::ostream &os_p, BoundingBox const &box_p)
{
	os_p<<"Box["
		<<"position: ("<<box_p.position[0]<<", "<<box_p.position[1]<<"), "
		<<"size: ("<<box_p.size[0]<<", "<<box_p.size[1]<<")]";
	return os_p;
}
