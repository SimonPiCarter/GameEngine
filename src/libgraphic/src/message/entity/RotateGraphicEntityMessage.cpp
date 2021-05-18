#include "RotateGraphicEntityMessage.h"

#include "message/GraphicMessageHandler.h"

RotateGraphicEntityMessage::RotateGraphicEntityMessage(GraphicEntity * entity_p, std::array<double, 3> const &vector_p)
	: GraphicMessage("")
	, _entity(entity_p)
	, _vector(vector_p)
{}

void RotateGraphicEntityMessage::visit(GraphicMessageHandler &handler_p)
{
	handler_p.visitRotateGraphicEntity(*this);
}
