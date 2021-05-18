#include "DestroyGraphicEntityMessage.h"

#include "message/GraphicMessageHandler.h"

DestroyGraphicEntityMessage::DestroyGraphicEntityMessage(GraphicEntity * entity_p)
	: GraphicMessage("")
	, _entity(entity_p)
{}

void DestroyGraphicEntityMessage::visit(GraphicMessageHandler &handler_p)
{
	handler_p.visitDestroyGraphicEntity(*this);
}
