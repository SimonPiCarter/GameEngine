#include "CustomGuiMessage.h"

#include "message/GraphicMessageHandler.h"

CustomGuiToolkit::~CustomGuiToolkit() {}

CustomGuiMessage::CustomGuiMessage(std::function<void(CustomGuiToolkit*, GraphicEngine *)> const &func_p, CustomGuiToolkit * toolkit_p, bool deleteToolkit_p)
	: GraphicMessage("")
	, _func(func_p)
	, _toolkit(toolkit_p)
	, _deleteToolkit(deleteToolkit_p)
{}

void CustomGuiMessage::visit(GraphicMessageHandler &handler_p)
{
	handler_p.visitCustomGui(*this);
}

DestroyWindowMessage::DestroyWindowMessage(Colibri::Window * window_p)
	: GraphicMessage("")
	, _window(window_p)
{}

void DestroyWindowMessage::visit(GraphicMessageHandler &handler_p)
{
	handler_p.visitDestroyWindow(*this);
}
