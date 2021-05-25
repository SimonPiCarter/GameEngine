#include "BlocForm.h"

#include <exception>

BlocForm::BlocForm(std::array<std::array<std::array<bool, 3>, 3>, 3> const &form_p)
	:_form(form_p)
{
	_minCorner = {4, 4, 4};
	_maxCorner = {0, 0, 0};
	for(size_t i = 0 ; i < 3 ; ++ i)
	{
		for(size_t j = 0 ; j < 3 ; ++ j)
		{
			for(size_t k = 0 ; k < 3 ; ++ k)
			{
				if(_form[i][j][k])
				{
					_minCorner[0] = std::min(_minCorner[0], i);
					_minCorner[1] = std::min(_minCorner[1], j);
					_minCorner[2] = std::min(_minCorner[2], k);
					_maxCorner[0] = std::max(_maxCorner[0], i);
					_maxCorner[1] = std::max(_maxCorner[1], j);
					_maxCorner[2] = std::max(_maxCorner[2], k);
				}
			}
		}
	}
	// check found one
	if(_minCorner[0] != 0 || _minCorner[1] != 0 || _minCorner[2] != 0)
	{
		throw std::logic_error("Invalid bloc model (min corner needs to be 0,0,0");
	}
}

std::array<std::array<std::array<bool, 3>, 3>, 3>const &BlocForm::getForm() const
{
	return _form;
}

std::array<size_t, 3> const &BlocForm::getMinCorner() const { return _minCorner; }
std::array<size_t, 3> const &BlocForm::getMaxCorner() const { return _maxCorner; }
