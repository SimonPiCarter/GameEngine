#include "MoveCameraMessage.h"

#include "message/GraphicMessageHandler.h"

MoveCameraMessage::MoveCameraMessage(std::array<double, 3> const &vector_p)
	: GraphicMessage("")
	, _vector(vector_p)
{}

void MoveCameraMessage::visit(GraphicMessageHandler &handler_p)
{
	handler_p.visitMoveCamera(*this);
}
