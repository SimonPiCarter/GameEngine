#include "BlocMessageHandler.h"

#include "bloc/Bloc.h"
#include "message/entity/DestroyGraphicEntityMessage.h"
#include "engine/BlocEngine.h"
#include "engine/GraphicEngine.h"

BlocMessageHandler::BlocMessageHandler(BlocEngine *engine_p)
	: _engine(engine_p)
{}

void BlocMessageHandler::visitPopBloc(PopBlocMessage const &msg_p)
{
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

	std::array<std::array<std::array<bool, 3>, 3>, 3> const & form_l = bloc_l->getModel().getForm();

	graphic_l.registerMessage(new NewSceneMessage("currentBloc", "main", {msg_p.getPosition()[0], msg_p.getPosition()[1], msg_p.getPosition()[2]}));
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
				graphic_l.registerMessage(new NewGraphicEntityMessage(entity_l, bloc_l->getModel().getMaterial(), {i,j,k}, {0.5,0.5,0.5}, "currentBloc"));
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

