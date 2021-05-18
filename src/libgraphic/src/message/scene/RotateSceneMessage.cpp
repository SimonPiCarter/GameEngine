#include "RotateSceneMessage.h"

#include "message/GraphicMessageHandler.h"

RotateSceneMessage::RotateSceneMessage(std::string const &id_p, std::array<double, 3> const &vector_p)
	: GraphicMessage("")
	, _id(id_p)
	, _vector(vector_p)
{}

void RotateSceneMessage::visit(GraphicMessageHandler &handler_p)
{
	handler_p.visitRotateScene(*this);
}
