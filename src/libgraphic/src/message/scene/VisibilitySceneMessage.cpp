#include "VisibilitySceneMessage.h"

#include "message/GraphicMessageHandler.h"

VisibilitySceneMessage::VisibilitySceneMessage(std::string const &id_p, bool visibility_p)
	: GraphicMessage("")
	, _id(id_p)
	, _visibility(visibility_p)
{}

void VisibilitySceneMessage::visit(GraphicMessageHandler &handler_p)
{
	handler_p.visitVisibilityScene(*this);
}
