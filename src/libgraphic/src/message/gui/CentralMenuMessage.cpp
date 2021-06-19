#include "CentralMenuMessage.h"

#include "message/GraphicMessageHandler.h"

NewCentralMenuMessage::NewCentralMenuMessage(CentralMenuVessel & menu_p, std::string const &title_p, std::vector<ButtonData> const &data_p)
	: GraphicMessage("")
	, _menu(menu_p)
	, _title(title_p)
	, _data(data_p)
{}

void NewCentralMenuMessage::visit(GraphicMessageHandler &handler_p)
{
	handler_p.visitNewCentralMenu(*this);
}

HideCentralMenuMessage::HideCentralMenuMessage(CentralMenuVessel & menu_p, bool hide_p)
	: GraphicMessage(""), _menu(menu_p), _hidden(hide_p)
{}

void HideCentralMenuMessage::visit(GraphicMessageHandler &handler_p)
{
	handler_p.visitHideCentralMenu(*this);
}
