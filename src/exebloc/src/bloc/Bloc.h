#pragma once

#include <array>

#include "BlocModel.h"

class GraphicEntity;

class Bloc
{
public:
	Bloc(BlocModel const &model_p, std::array<unsigned long, 3> const &position_p, double level_p);

	/// @brief update the level will (will also update _position[2])
	void updateLevel(double level_p);

	/// @brief update _position[0]
	void setX(unsigned long x);

	/// @brief update _position[1]
	void setY(unsigned long y);

	/// @brief _model getter
	BlocModel const &getModel() const;

	/// @brief _position getter
	std::array<unsigned long, 3> const &getPosition() const;

	/// @brief _level getter
	double const &getLevel() const;

	std::array<std::array<std::array<GraphicEntity *, 3>, 3>, 3> & getEntities();
	std::array<std::array<std::array<GraphicEntity *, 3>, 3>, 3> const & getEntities() const;

protected:
	/// @brief reference to the model
	BlocModel const & _model;
	/// @brief the position of the bloc
	std::array<unsigned long, 3> _position;
	/// @brief the height level of the bloc (_position[2] == std::ceil(_level))
	double _level;
	/// @brief the pointers to GraphicEntity (used to load the bloc in the map)
	std::array<std::array<std::array<GraphicEntity *, 3>, 3>, 3> _entities;
};
