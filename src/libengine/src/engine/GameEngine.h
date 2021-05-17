#pragma once

#include "message/game/GameMessageHandler.h"

#include <engine/GraphicEngine.h>
#include <resource/Resource.h>

class GameEngine : public GameMessageHandler
{
public:
	GameEngine();
	virtual ~GameEngine();

	virtual void init() = 0;
	virtual void run() = 0;

protected:
	ResourceHandler _resourceHandler;
	GraphicEngine _graphic;
};
