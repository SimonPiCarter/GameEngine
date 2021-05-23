#include "BlocMap.h"

#include <cassert>
#include <set>

#include "bloc/Bloc.h"
#include "bloc/BlocModel.h"
#include "message/BlocMessage.h"
#include "engine/BlocEngine.h"

BlocMap::BlocMap(BlocEngine *engine_p)
	: _engine(engine_p)
{
	for(size_t i = 0 ; i < 9 ; ++ i)
	{
		for(size_t j = 0 ; j < 9 ; ++ j)
		{
			for(size_t k = 0 ; k < 10 ; ++ k)
			{
				_map[i][j][k] = nullptr;
			}
		}
	}
}

// Add the bloc in the map (may trigger removal of bloc)
void BlocMap::addBloc(GraphicEntity *entity_p, std::array<size_t, 3> const &pos_p)
{
	// add entity
	assert(!_map[pos_p[0]][pos_p[1]][pos_p[2]]);
	_map[pos_p[0]][pos_p[1]][pos_p[2]] = entity_p;

	std::set<GraphicEntity*> entityToPop_l;

	// check for full lines X
	bool checkX_l = true;
	for(size_t i = 0; i < 9; ++i)
	{
		if(!_map[i][pos_p[1]][pos_p[2]])
		{
			checkX_l = false;
			break;
		}
	}
	if(checkX_l)
	{
		// clear map
		for(size_t i = 0; i < 9; ++i)
		{
			entityToPop_l.insert(_map[i][pos_p[1]][pos_p[2]]);
			_map[i][pos_p[1]][pos_p[2]] = nullptr;
		}
	}
	// check for full lines X
	bool checkY_l = true;
	for(size_t i = 0; i < 9; ++i)
	{
		if(!_map[pos_p[0]][i][pos_p[2]])
		{
			checkY_l = false;
			break;
		}
	}
	if(checkY_l)
	{
		// clear map
		for(size_t i = 0; i < 9; ++i)
		{
			entityToPop_l.insert(_map[pos_p[0]][i][pos_p[2]]);
			_map[pos_p[0]][i][pos_p[2]] = nullptr;
		}
	}

	if(!entityToPop_l.empty())
	{
		_engine->MessageHandler<BlocMessage>::registerMessage(new PopBlocMessage(entityToPop_l));
	}
}

/// @brief check that position is free for bloc at given position
bool BlocMap::checkPosition(Bloc * bloc_p, std::array<unsigned long, 3> const &position_p) const
{
	std::array<std::array<std::array<bool, 3>, 3>, 3> const & form_l = bloc_p->getModel().getForm();

	assert(position_p[0] + bloc_p->getModel().getMaxCorner()[0] < 9);
	assert(position_p[1] + bloc_p->getModel().getMaxCorner()[1] < 9);
	assert(position_p[1] + bloc_p->getModel().getMaxCorner()[2] < 10);

	// for each sub bloc in the form
	for(size_t i = 0 ; i < 3 ; ++ i)
	{
		for(size_t j = 0 ; j < 3 ; ++ j)
		{
			for(size_t k = 0 ; k < 3 ; ++ k)
			{
				if(form_l[i][j][k]
				&& _map[i+position_p[0]][j+position_p[1]][k+position_p[2]])
				{
					return false;
				}
			}
		}
	}
	return true;
}

/// @brief check if the bloc has to freeze or not
bool BlocMap::checkFreeze(Bloc * bloc_p) const
{
	return bloc_p->getLevel() + bloc_p->getModel().getMaxCorner()[2] > 9.95
		|| !checkPosition(bloc_p, bloc_p->getPosition());
}

/// @brief check if the game is over
bool BlocMap::checkLose() const
{
	// check 10th level for any bloc
	for(size_t i = 0; i < 9; ++i)
	{
		for(size_t j = 0; j < 9; ++j)
		{
			if(_map[i][j][0])
			{
				return true;
			}
		}
	}
	return false;
}
