#include "ArrayUtils.h"

#include <cmath>

std::array<double, 2> normalize(std::array<double, 2> const &array_p)
{
	double length_l = sqrt(array_p[0]*array_p[0] + array_p[1]*array_p[1]);
	return { array_p[0]/length_l, array_p[1]/length_l };
}
