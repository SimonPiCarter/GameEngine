#include "RichLabelMessage.h"

#include "message/GraphicMessageHandler.h"

NewRichLabelMessage::NewRichLabelMessage(RichLabelVessel & label_p, std::vector<InfoLabel> const &content_p,
	double x_p, double y_p, double width_p, double height_p, float size_p, bool back_p)
	: GraphicMessage("")
	, _label(label_p)
	, _content(content_p)
	, _x(x_p)
	, _y(y_p)
	, _width(width_p)
	, _height(height_p)
	, _size(size_p)
	, _back(back_p)
{}

void NewRichLabelMessage::visit(GraphicMessageHandler &handler_p)
{
	handler_p.visitNewRichLabel(*this);
}


HideRichLabelMessage::HideRichLabelMessage(RichLabelVessel & label_p, bool hide_p)
	: GraphicMessage(""), _label(label_p), _hidden(hide_p)
{}

void HideRichLabelMessage::visit(GraphicMessageHandler &handler_p)
{
	handler_p.visitHideRichLabel(*this);
}

UpdateTextRichLabelMessage::UpdateTextRichLabelMessage(RichLabelVessel & label_p, std::vector<InfoLabel> const &content_p)
	: GraphicMessage("")
	, _label(label_p)
	, _content(content_p)
{}

void UpdateTextRichLabelMessage::visit(GraphicMessageHandler &handler_p)
{
	handler_p.visitUpdateTextRichLabel(*this);
}

SetPositionRichLabelMessage::SetPositionRichLabelMessage(RichLabelVessel & label_p, double x, double y)
	: GraphicMessage("")
	, _label(label_p)
	, _x(x)
	, _y(y)
{}

void SetPositionRichLabelMessage::visit(GraphicMessageHandler &handler_p)
{
	handler_p.visitSetPositionRichLabel(*this);
}
