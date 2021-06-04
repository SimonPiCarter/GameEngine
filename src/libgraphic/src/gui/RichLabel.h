#pragma once

#include <string>
#include <vector>

#include "ColibriGui/ColibriManager.h"
#include "ColibriGui/ColibriWindow.h"

class GraphicEngine;

struct InfoLabel
{
	std::string text;
	int r;
	int g;
	int b;
};

/// @brief This class allow for rich Label handling
/// with coloring inside it
class RichLabel
{
public:
	RichLabel(std::vector<InfoLabel> const &content_p, double x, double y, double width, double height, float size_p, bool back_p, GraphicEngine &graphic_p);
	~RichLabel();

	void updateText(std::vector<InfoLabel> const &content_p);

protected:
	Colibri::ColibriManager * _manager;
	Colibri::Window * _window;
	Colibri::Label *_label;
};
