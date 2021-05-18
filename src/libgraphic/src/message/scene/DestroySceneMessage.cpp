#include "DestroySceneMessage.h"

#include "message/GraphicMessageHandler.h"

DestroySceneMessage::DestroySceneMessage(std::string const &id_p)
	: GraphicMessage("")
	, _id(id_p)
{}

void DestroySceneMessage::visit(GraphicMessageHandler &handler_p)
{
	handler_p.visitDestroyScene(*this);
}
