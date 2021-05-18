#include "RotateLightMessage.h"

#include "message/GraphicMessageHandler.h"

RotateLightMessage::RotateLightMessage(GraphicEntity * entity_p, std::array<double, 3> const &vector_p)
	: GraphicMessage("")
	, _entity(entity_p)
	, _vector(vector_p)
{}

void RotateLightMessage::visit(GraphicMessageHandler &handler_p)
{
	handler_p.visitRotateLight(*this);
}
