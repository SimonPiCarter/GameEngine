#include "AnimateGraphicEntityMessage.h"

#include "message/GraphicMessageHandler.h"

AnimateGraphicEntityMessage::AnimateGraphicEntityMessage(GraphicEntity * entity_p, std::string const &animation_p, bool loop_p, bool enable_p)
	: GraphicMessage("")
	, _entity(entity_p)
	, _animation(animation_p)
	, _loop(loop_p)
	, _enable(enable_p)
{}

void AnimateGraphicEntityMessage::visit(GraphicMessageHandler &handler_p)
{
	handler_p.visitAnimateGraphicEntity(*this);
}
