#pragma once

#include <array>
#include <list>

typedef std::array<std::array<std::array<bool, 3>, 3>, 3> pattern_t;

std::list<std::list<pattern_t>> readData(std::string const &fileName_p);
