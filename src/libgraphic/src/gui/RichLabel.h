#pragma once

#include <vector>

#include "ColibriGui/ColibriManager.h"
#include "ColibriGui/ColibriWindow.h"

#include "gui/InfoLabel.h"

class GraphicEngine;


/// @brief This class allow for rich Label handling
/// with coloring inside it
class RichLabel
{
public:
	RichLabel(std::vector<InfoLabel> const &content_p, double x, double y, double width, double height, float size_p, bool back_p, GraphicEngine &graphic_p);
	~RichLabel();

	void setHidden(bool hidden_p);

	void updateText(std::vector<InfoLabel> const &content_p);

	void setPosition(double x, double y);

	Colibri::Window * getWindow() { return _window; }

protected:
	Colibri::ColibriManager * _manager;
	Colibri::Window * _window;
	Colibri::Label *_label;
};
