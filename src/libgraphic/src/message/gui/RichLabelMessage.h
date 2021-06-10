#pragma once

#include "message/GraphicMessage.h"
#include "gui/RichLabel.h"

#include <vector>

struct RichLabelVessel
{
	~RichLabelVessel() { delete label; }
	RichLabel * label = nullptr;
};

class NewRichLabelMessage : public GraphicMessage
{
public:
	NewRichLabelMessage(RichLabelVessel & label_p, std::vector<InfoLabel> const &content_p,
		double x_p, double y_p, double width, double height, float size_p, bool back_p);

	virtual void visit(GraphicMessageHandler &handler_p);

	RichLabelVessel & getLabel() const { return _label; }
	std::vector<InfoLabel> const & getContent() const { return _content; }
	double getX() const { return _x; }
	double getY() const { return _y; }
	double getWidth() const { return _width; }
	double getHeight() const { return _height; }
	float getSize() const { return _size; }
	bool getBack() const { return _back; }

private:
	RichLabelVessel & _label;
	std::vector<InfoLabel> const _content;
	double const _x;
	double const _y;
	double const _width;
	double const _height;
	float const _size;
	bool const _back;
};

class HideRichLabelMessage : public GraphicMessage
{
public:
	HideRichLabelMessage(RichLabelVessel & label_p, bool hide_p);

	virtual void visit(GraphicMessageHandler &handler_p);

	RichLabelVessel & getLabel() const { return _label; }
	bool isHidden() const { return _hidden; }
private:
	RichLabelVessel & _label;
	bool const _hidden;
};

class UpdateTextRichLabelMessage : public GraphicMessage
{
public:
	UpdateTextRichLabelMessage(RichLabelVessel & label_p, std::vector<InfoLabel> const &content_p);

	virtual void visit(GraphicMessageHandler &handler_p);

	RichLabelVessel & getLabel() const { return _label; }
	std::vector<InfoLabel> const & getContent() const { return _content; }
private:
	RichLabelVessel & _label;
	std::vector<InfoLabel> const _content;
};

class SetPositionRichLabelMessage : public GraphicMessage
{
public:
	SetPositionRichLabelMessage(RichLabelVessel & label_p, double x, double y);

	virtual void visit(GraphicMessageHandler &handler_p);

	RichLabelVessel & getLabel() const { return _label; }
	double getX() const { return _x; }
	double getY() const { return _y; }
private:
	RichLabelVessel & _label;
	double _x;
	double _y;
};
