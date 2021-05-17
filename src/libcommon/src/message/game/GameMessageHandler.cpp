#include "GameMessageHandler.h"

#include <iostream>

GameMessageHandler::GameMessageHandler(GameEngine *engine_p) : _engine(engine_p) {}
GameMessageHandler::~GameMessageHandler() {}

void GameMessageHandler::handleMessage(GameMessage *msg_p)
{
	msg_p->visit(*this);
}

void GameMessageHandler::visitNoOp(NoOpGameMessage const &)
{
	// NA
}
