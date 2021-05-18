#include "NewLightMessage.h"

#include "message/GraphicMessageHandler.h"

NewLightMessage::NewLightMessage(GraphicEntity * entity_p, std::string const &parent_p, const std::array<double, 3> & pos_p,
	const std::array<double, 3> & dir_p, LightType type_p)
	: GraphicMessage("")
	, _entity(entity_p)
	, _parent(parent_p)
	, _position(pos_p)
	, _direction(dir_p)
	, _type(type_p)
{}

void NewLightMessage::visit(GraphicMessageHandler &handler_p)
{
	handler_p.visitNewLight(*this);
}
