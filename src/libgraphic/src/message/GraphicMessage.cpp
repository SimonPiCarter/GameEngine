#include "GraphicMessage.h"

#include "GraphicMessageHandler.h"

GraphicMessage::GraphicMessage(std::string const &msg_p)
	: _msg(msg_p)
{}

GraphicMessage::~GraphicMessage()
{}

std::string const & GraphicMessage::getMsg() const
{
	return _msg;
}

NoOpGraphicMessage::NoOpGraphicMessage(std::string const &msg_p)
	: GraphicMessage(msg_p)
{}

void NoOpGraphicMessage::visit(GraphicMessageHandler &handler_p)
{
	handler_p.visitNoOp(*this);
}
