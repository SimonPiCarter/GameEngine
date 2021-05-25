#include "DataReader.h"

#include <cassert>
#include <fstream>
#include <sstream>

std::list<std::list<pattern_t>> readData(std::string const &fileName_p)
{
	std::ifstream file_l(fileName_p);

	if(!file_l.is_open())
	{
		throw std::logic_error("Cannot open file "+fileName_p);
	}

	std::list<std::list<pattern_t>> result_l;
	std::list<pattern_t> * currentModel_l = nullptr;
	pattern_t * currentPattern_l = nullptr;
	size_t lineIdx_l = 0;


	std::string line_l;
	while (std::getline(file_l, line_l))
	{
		std::istringstream ss_l(line_l);
		// new model
		if(line_l == ">")
		{
			result_l.push_back(std::list<pattern_t>());
			currentModel_l = &result_l.back();
			currentModel_l->push_back(pattern_t());
			currentPattern_l = &currentModel_l->back();
			lineIdx_l = 0;
		}
		// new form
		else if(line_l == "=")
		{
			assert(currentModel_l);
			currentModel_l->push_back(pattern_t());
			currentPattern_l = &currentModel_l->back();
			lineIdx_l = 0;
		}
		else
		{
			int digit_l;
			for(size_t i = 0 ; i < 3 ; ++ i)
			{
				ss_l >> digit_l;
				if(!ss_l >> digit_l)
				{
					throw std::logic_error("cannot parse file "+fileName_p);
				}
				(*currentPattern_l)[lineIdx_l][0][i] = digit_l > 0;
			}
			// increment line pattern
			++lineIdx_l;
		}
	}

	return result_l;
}
