#include "MoveLightMessage.h"

#include "message/GraphicMessageHandler.h"

MoveLightMessage::MoveLightMessage(GraphicEntity * entity_p, std::array<double, 3> const &vector_p)
	: GraphicMessage("")
	, _entity(entity_p)
	, _vector(vector_p)
{}

void MoveLightMessage::visit(GraphicMessageHandler &handler_p)
{
	handler_p.visitMoveLight(*this);
}
