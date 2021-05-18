#include "MoveSceneMessage.h"

#include "message/GraphicMessageHandler.h"

MoveSceneMessage::MoveSceneMessage(std::string const &id_p, std::array<double, 3> const &vector_p)
	: GraphicMessage("")
	, _id(id_p)
	, _vector(vector_p)
{}

void MoveSceneMessage::visit(GraphicMessageHandler &handler_p)
{
	handler_p.visitMoveScene(*this);
}
