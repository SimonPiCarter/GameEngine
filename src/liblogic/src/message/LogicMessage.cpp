#include "LogicMessage.h"

#include "LogicMessageHandler.h"

LogicMessage::LogicMessage(std::string const &msg_p)
	: _msg(msg_p)
{}

LogicMessage::~LogicMessage()
{}

std::string const & LogicMessage::getMsg() const
{
	return _msg;
}

NoOpLogicMessage::NoOpLogicMessage(std::string const &msg_p)
	: LogicMessage(msg_p)
{}

void NoOpLogicMessage::visit(LogicMessageHandler &handler_p)
{
	handler_p.visitNoOp(*this);
}
