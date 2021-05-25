#include "BlocMessageHandler.h"

#include "bloc/Bloc.h"
#include "message/entity/DestroyGraphicEntityMessage.h"
#include "message/entity/MoveGraphicEntityMessage.h"
#include "engine/BlocEngine.h"
#include "engine/GraphicEngine.h"

BlocMessageHandler::BlocMessageHandler(BlocEngine *engine_p)
	: _engine(engine_p)
{}

void BlocMessageHandler::visitPopBloc(PopBlocMessage const &msg_p)
{
	// Redirect to map to move down all blocs and get the affected entities
	std::set<GraphicEntity *> entitiesToDrop_l = _engine->getMap().popBloc(msg_p.getPos(), msg_p.getPopX(), msg_p.getPopY());
	// Move all enitities
	for(GraphicEntity *entity_l : entitiesToDrop_l)
	{
		_engine->getGraphic().registerMessage(new MoveGraphicEntityMessage(entity_l, {0,0,1}));
	}
	// Remove all entities to pop
	for(GraphicEntity *entity_l : msg_p.getSet())
	{
		_engine->getGraphic().registerMessage(new DestroyGraphicEntityMessage(entity_l));
	}
}

void BlocMessageHandler::visitSpawnBloc(SpawnBlocMessage const &msg_p)
{
	// Create bloc
	Bloc * bloc_l = new Bloc(msg_p.getModel(), msg_p.getPosition(), msg_p.getPosition()[2]);
	_engine->setCurrentBloc(bloc_l);

	GraphicEngine &graphic_l(_engine->getGraphic());

	std::array<std::array<std::array<bool, 3>, 3>, 3> const & form_l = bloc_l->getForm().getForm();

	graphic_l.registerMessage(new NewSceneMessage("currentBloc", "main",
		{double(msg_p.getPosition()[0]), double(msg_p.getPosition()[1]), double(msg_p.getPosition()[2])}));
	for(size_t i = 0 ; i < 3 ; ++ i)
	{
		for(size_t j = 0 ; j < 3 ; ++ j)
		{
			for(size_t k = 0 ; k < 3 ; ++ k)
			{
				if(!form_l[i][j][k])
				{
					continue;
				}
				GraphicEntity * entity_l = new GraphicEntity();
				graphic_l.registerMessage(new NewGraphicEntityMessage(entity_l, bloc_l->getModel().getMaterial()
					, {double(i),double(j),double(k)}, {0.5,0.5,0.5}, "currentBloc"));
				bloc_l->getEntities()[i][j][k] = entity_l;
			}
		}
	}
}

void BlocMessageHandler::visitFreezeBloc(FreezeBlocMessage const &msg_p)
{
	Bloc * bloc_l = msg_p.getBloc();
	for(size_t i = 0 ; i < 3 ; ++ i)
	{
		for(size_t j = 0 ; j < 3 ; ++ j)
		{
			for(size_t k = 0 ; k < 3 ; ++ k)
			{
				if(!bloc_l->getEntities()[i][j][k])
				{
					continue;
				}
				_engine->getMap().addBloc(bloc_l->getEntities()[i][j][k]
					, {bloc_l->getPosition()[0]+i,bloc_l->getPosition()[1]+j,bloc_l->getPosition()[2]+k});
			}
		}
	}
	delete bloc_l;
}

namespace
{

	struct GraphicData
	{
		std::array<size_t, 3> old_pos;
		GraphicEntity * entity;
	};

	std::list<GraphicData> getData(Bloc const *bloc_p)
	{
		std::list<GraphicData> list_l;
		for(size_t i = 0 ; i < 3 ; ++ i)
		{
			for(size_t j = 0 ; j < 3 ; ++ j)
			{
				for(size_t k = 0 ; k < 3 ; ++ k)
				{
					if(bloc_p->getEntities()[i][j][k])
					{
						// store entity with old position
						list_l.push_back({{i,j,k}, bloc_p->getEntities()[i][j][k]});
					}
				}
			}
		}
		return list_l;
	}

	void resetEntities(Bloc * bloc_p)
	{
		for(size_t i = 0 ; i < 3 ; ++ i)
		{
			for(size_t j = 0 ; j < 3 ; ++ j)
			{
				for(size_t k = 0 ; k < 3 ; ++ k)
				{
					bloc_p->getEntities()[i][j][k] = nullptr;
				}
			}
		}
	}
}
void BlocMessageHandler::visitRotateBloc(RotateBlocMessage const &msg_p)
{
	Bloc * bloc_l = msg_p.getBloc();
	size_t nextIndex_l = (bloc_l->getFormIndex() + 1) % bloc_l->getModel().getForms().size();

	bool found_l = false;
	// While we have not found a valid index
	while(!found_l && bloc_l->getFormIndex() != nextIndex_l)
	{
		if(!_engine->getMap().checkFreeze(bloc_l, &bloc_l->getModel().getForms().at(nextIndex_l)))
		{
			found_l = true;
		} else
		{
			nextIndex_l = (nextIndex_l + 1) % bloc_l->getModel().getForms().size();
		}
	}

	bloc_l->setFormIndex(nextIndex_l);
	BlocForm const &newForm_l = bloc_l->getForm();
	// update graphic
	std::list<GraphicData> list_l = getData(bloc_l);
	resetEntities(bloc_l);
	for(size_t i = 0 ; i < 3 ; ++ i)
	{
		for(size_t j = 0 ; j < 3 ; ++ j)
		{
			for(size_t k = 0 ; k < 3 ; ++ k)
			{
				if(newForm_l.getForm()[i][j][k])
				{
					// store back in bloc
					GraphicData data_l = list_l.front();
					list_l.pop_front();
					bloc_l->getEntities()[i][j][k] = data_l.entity;
					// move bloc
					// IMPORTANT to use relative transformation to avoid desync between level and position
					_engine->getGraphic().registerMessage(new MoveGraphicEntityMessage(data_l.entity,
						{
							double(i) - double(data_l.old_pos[0]),
							double(j) - double(data_l.old_pos[1]),
							double(k) - double(data_l.old_pos[2]),
						}));
				}
			}
		}
	}
}
