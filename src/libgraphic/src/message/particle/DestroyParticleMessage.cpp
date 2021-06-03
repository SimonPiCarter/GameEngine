#include "DestroyParticleMessage.h"

#include "message/GraphicMessageHandler.h"

DestroyParticleMessage::DestroyParticleMessage(GraphicEntity * entity_p)
	: GraphicMessage("")
	, _entity(entity_p)
{}

void DestroyParticleMessage::visit(GraphicMessageHandler &handler_p)
{
	handler_p.visitDestroyParticle(*this);
}
