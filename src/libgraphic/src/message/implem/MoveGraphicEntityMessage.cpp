#include "MoveGraphicEntityMessage.h"

#include "message/GraphicMessageHandler.h"

MoveGraphicEntityMessage::MoveGraphicEntityMessage(GraphicEntity * entity_p, std::array<double, 3> const &vector_p)
	: GraphicMessage("")
	, _entity(entity_p)
	, _vector(vector_p)
{}

void MoveGraphicEntityMessage::visit(GraphicMessageHandler &handler_p)
{
	handler_p.visitMoveGraphicEntity(*this);
}
