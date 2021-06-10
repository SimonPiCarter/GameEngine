#include "CustomGuiMessage.h"

#include "message/GraphicMessageHandler.h"

CustomGuiToolkit::~CustomGuiToolkit() {}

CustomGuiMessage::CustomGuiMessage(std::function<void(CustomGuiToolkit*, GraphicEngine *)> const &func_p, CustomGuiToolkit * toolkit_p)
	: GraphicMessage("")
	, _func(func_p)
	, _toolkit(toolkit_p)
{}

void CustomGuiMessage::visit(GraphicMessageHandler &handler_p)
{
	handler_p.visitCustomGui(*this);
}
