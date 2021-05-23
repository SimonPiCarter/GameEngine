#pragma once

#include <array>

class Bloc;
class BlocEngine;
class GraphicEntity;

class BlocMap
{
public:
	BlocMap(BlocEngine *engine_p);

	// Add the bloc in the map (may trigger removal of bloc)
	void addBloc(GraphicEntity *entity_p, std::array<size_t, 3> const &pos_p);

	/// @brief check that position is free for bloc at given position
	bool checkPosition(Bloc * bloc_p, std::array<unsigned long, 3> const &position_p) const;

	/// @brief check if the bloc has to freeze or not
	bool checkFreeze(Bloc * bloc_p) const;

	/// @brief check if the game is over
	bool checkLose() const;

protected:
	/// @brief the material for each bloc in the map (if nullptr no bloc)
	std::array<std::array<std::array<GraphicEntity *, 9>, 9>, 10> _map;

	/// @brief pointer to the engine to send messages
	BlocEngine * const _engine;
};
