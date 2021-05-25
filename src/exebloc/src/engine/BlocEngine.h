#pragma once

#include "engine/GraphicEngine.h"
#include "resource/Resource.h"
#include "entity/GraphicEntity.h"

#include "engine/GameEngine.h"
#include "gui/CentralMenu.h"
#include "gui/RichLabel.h"

#include "message/BlocMessageHandler.h"

#include "bloc/BlocMap.h"

class Bloc;

class BlocEngine : public GameEngine, public BlocMessageHandler
{
public:
	BlocEngine(std::string const &path_p);

	virtual void init() override;
	virtual void run() override;

	virtual void visitSDLEvent(SDLEventGameMessage const &msg_p) override;

	GraphicEngine & getGraphic() { return _graphic; }

	void setCurrentBloc(Bloc *bloc_p) { _currentBloc = bloc_p; }
	BlocMap & getMap() { return _map; }

private:
	Bloc * _currentBloc = nullptr;

	BlocMap _map;

	bool _speed = false;
};
