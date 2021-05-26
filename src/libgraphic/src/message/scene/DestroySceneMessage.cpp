#include "DestroySceneMessage.h"

#include "message/GraphicMessageHandler.h"

DestroySceneMessage::DestroySceneMessage(std::string const &id_p, bool destroyChildren_p)
	: GraphicMessage("")
	, _id(id_p)
	, _destroyChildren(destroyChildren_p)
{}

void DestroySceneMessage::visit(GraphicMessageHandler &handler_p)
{
	handler_p.visitDestroyScene(*this);
}
