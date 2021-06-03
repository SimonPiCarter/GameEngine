#include "NewParticleMessage.h"

#include "message/GraphicMessageHandler.h"

NewParticleMessage::NewParticleMessage(GraphicEntity * entity_p, std::string const &resourceId_p, const std::array<double, 3> & pos_p, std::string const &scene_p)
	: GraphicMessage("")
	, _entity(entity_p)
	, _resourceId(resourceId_p)
	, _position(pos_p)
	, _scene(scene_p)
{}

void NewParticleMessage::visit(GraphicMessageHandler &handler_p)
{
	handler_p.visitNewParticle(*this);
}
