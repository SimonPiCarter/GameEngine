#pragma once

#include <message/MessageHandler.h>

#include "GameMessage.h"

class GameEngine;

class GameMessageHandler : public MessageHandler<GameMessage>
{
public:
	GameMessageHandler(GameEngine *engine_p);
	~GameMessageHandler();

	// visit methods
	// NoOp (used in test)
	void visitNoOp(NoOpGameMessage const &msg_p);

	// sdl event (given by graphic engine)
	virtual void visitSDLEvent(SDLEventGameMessage const &msg_p) = 0;

protected:
	/// @brief method to be extended to handle message
	virtual void handleMessage(GameMessage *msg_p) override;

	GameEngine * const _engine;
};
