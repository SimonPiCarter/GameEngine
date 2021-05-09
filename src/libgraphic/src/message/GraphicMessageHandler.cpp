#include "GraphicMessageHandler.h"

#include <iostream>

GraphicMessageHandler::GraphicMessageHandler() {}
GraphicMessageHandler::~GraphicMessageHandler() {}

void GraphicMessageHandler::handleMessage(GraphicMessage *msg_p)
{
	msg_p->visit(*this);
}

void GraphicMessageHandler::visitNoOp(NoOpGraphicMessage &)
{
	// NA
}
