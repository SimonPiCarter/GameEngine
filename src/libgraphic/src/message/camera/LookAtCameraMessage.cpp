#include "LookAtCameraMessage.h"

#include "message/GraphicMessageHandler.h"

LookAtCameraMessage::LookAtCameraMessage(std::array<double, 3> const &vector_p)
	: GraphicMessage("")
	, _vector(vector_p)
{}

void LookAtCameraMessage::visit(GraphicMessageHandler &handler_p)
{
	handler_p.visitLookAtCamera(*this);
}
