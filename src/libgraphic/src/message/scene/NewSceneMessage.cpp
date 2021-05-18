#include "NewSceneMessage.h"

#include "message/GraphicMessageHandler.h"

NewSceneMessage::NewSceneMessage(std::string const &id_p, std::string const &parent_p, const std::array<double, 3> & pos_p)
	: GraphicMessage("")
	, _id(id_p)
	, _parent(parent_p)
	, _position(pos_p)
{}

void NewSceneMessage::visit(GraphicMessageHandler &handler_p)
{
	handler_p.visitNewScene(*this);
}
