#include "DestroyLightMessage.h"

#include "message/GraphicMessageHandler.h"

DestroyLightMessage::DestroyLightMessage(GraphicEntity * entity_p)
	: GraphicMessage("")
	, _entity(entity_p)
{}

void DestroyLightMessage::visit(GraphicMessageHandler &handler_p)
{
	handler_p.visitDestroyLight(*this);
}
