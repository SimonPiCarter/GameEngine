#include "RotateCameraMessage.h"

#include "message/GraphicMessageHandler.h"

RotateCameraMessage::RotateCameraMessage(std::array<double, 3> const &vector_p)
	: GraphicMessage("")
	, _vector(vector_p)
{}

void RotateCameraMessage::visit(GraphicMessageHandler &handler_p)
{
	handler_p.visitRotateCamera(*this);
}
