#include "Bloc.h"

#include <cmath>

Bloc::Bloc(BlocModel const &model_p, std::array<unsigned long, 3> const &position_p, double level_p)
	: _model(model_p), _position(position_p), _level(level_p)
{
	for(size_t i = 0 ; i < 3 ; ++ i)
	{
		for(size_t j = 0 ; j < 3 ; ++ j)
		{
			for(size_t k = 0 ; k < 3 ; ++ k)
			{
				_entities[i][j][k] = nullptr;
			}
		}
	}
}

/// @brief update the level will (will also update _position[2])
void Bloc::updateLevel(double level_p)
{
	_level = level_p;
	_position[2] = std::ceil(_level - 1e-3);
}

/// @brief update _position[0]
void Bloc::setX(unsigned long x)
{
	_position[0] = x;
}

/// @brief update _position[1]
void Bloc::setY(unsigned long y)
{
	_position[1] = y;
}

/// @brief _model getter
BlocModel const &Bloc::getModel() const
{
	return _model;
}

/// @brief _position getter
std::array<unsigned long, 3> const &Bloc::getPosition() const
{
	return _position;
}

/// @brief _level getter
double const &Bloc::getLevel() const
{
	return _level;
}

std::array<std::array<std::array<GraphicEntity *, 3>, 3>, 3> & Bloc::getEntities()
{
	return _entities;
}
std::array<std::array<std::array<GraphicEntity *, 3>, 3>, 3> const & Bloc::getEntities() const
{
	return _entities;
}
