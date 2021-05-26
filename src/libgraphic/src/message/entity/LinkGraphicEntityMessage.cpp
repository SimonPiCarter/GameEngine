#include "LinkGraphicEntityMessage.h"

#include "message/GraphicMessageHandler.h"

LinkGraphicEntityMessage::LinkGraphicEntityMessage(GraphicEntity * entity_p, std::string const &scene_p)
	: GraphicMessage("")
	, _entity(entity_p)
	, _scene(scene_p)
{}

void LinkGraphicEntityMessage::visit(GraphicMessageHandler &handler_p)
{
	handler_p.visitLinkGraphicEntity(*this);
}
