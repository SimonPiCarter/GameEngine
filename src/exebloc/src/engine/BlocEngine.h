#pragma once

#include "engine/GraphicEngine.h"
#include "resource/Resource.h"
#include "entity/GraphicEntity.h"

#include "engine/GameEngine.h"
#include "gui/CentralMenu.h"
#include "gui/RichLabel.h"

#include "message/BlocMessageHandler.h"
#include "BlocType.h"

#include "bloc/BlocMap.h"

class Bloc;

std::string getScene(BlocType type_p);

class BlocEngine : public GameEngine, public BlocMessageHandler
{
public:
	BlocEngine(std::string const &path_p);

	virtual void init() override;
	virtual void run() override;

	virtual void visitSDLEvent(SDLEventGameMessage const &msg_p) override;

	GraphicEngine & getGraphic() { return _graphic; }

	void setBloc(BlocType const &type_p, Bloc *bloc_p) { _mapBlocs[type_p] = bloc_p; }
	BlocMap & getMap() { return _map; }

	Bloc * getCurrentBloc() { return _mapBlocs[BlocType::CURRENT_BLOC]; }
	Bloc * getNextBloc() { return _mapBlocs[BlocType::NEXT_BLOC]; }

private:
	std::unordered_map<BlocType, Bloc *> _mapBlocs;
	BlocModel * _nextModel = nullptr;

	BlocMap _map;

	bool _speed = false;
};
