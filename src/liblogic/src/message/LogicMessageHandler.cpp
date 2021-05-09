#include "LogicMessageHandler.h"

#include <iostream>

LogicMessageHandler::LogicMessageHandler() {}
LogicMessageHandler::~LogicMessageHandler() {}

void LogicMessageHandler::handleMessage(LogicMessage *msg_p)
{
	msg_p->visit(*this);
}

void LogicMessageHandler::visitNoOp(NoOpLogicMessage &)
{
	// NA
}
