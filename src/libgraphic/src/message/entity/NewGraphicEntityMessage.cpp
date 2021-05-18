#include "NewGraphicEntityMessage.h"

#include "message/GraphicMessageHandler.h"

NewGraphicEntityMessage::NewGraphicEntityMessage(GraphicEntity * entity_p, std::string const &resourceId_p,
	std::array<double, 3> const &pos_p, std::array<double, 3> const &scale_p, std::string const &scene_p)
	: GraphicMessage("")
	, _entity(entity_p)
	, _resourceId(resourceId_p)
	, _position(pos_p)
	, _scale(scale_p)
	, _scene(scene_p)
{}

void NewGraphicEntityMessage::visit(GraphicMessageHandler &handler_p)
{
	handler_p.visitNewGraphicEntity(*this);
}
