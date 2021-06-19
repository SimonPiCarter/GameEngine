#include "DestroyGraphicEntityMessage.h"

#include "message/GraphicMessageHandler.h"

DestroyGraphicEntityMessage::DestroyGraphicEntityMessage(GraphicEntity * entity_p, bool delete_p)
	: GraphicMessage("")
	, _entity(entity_p)
	, _delete(delete_p)
{}

void DestroyGraphicEntityMessage::visit(GraphicMessageHandler &handler_p)
{
	handler_p.visitDestroyGraphicEntity(*this);
}
