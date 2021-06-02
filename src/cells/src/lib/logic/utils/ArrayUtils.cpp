#include "ArrayUtils.h"

#include <cmath>

std::array<double, 2> normalize(std::array<double, 2> const &array_p)
{
	double length_l = sqrt(array_p[0]*array_p[0] + array_p[1]*array_p[1]);
	return { array_p[0]/length_l, array_p[1]/length_l };
}

double square_distance(std::array<double, 2> const &pos1_p, std::array<double, 2> const &pos2_p)
{
	return (pos1_p[0]-pos2_p[0])*(pos1_p[0]-pos2_p[0]) + (pos1_p[1]-pos2_p[1])*(pos1_p[1]-pos2_p[1]);
}
