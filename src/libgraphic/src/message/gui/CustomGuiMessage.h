#pragma once

#include "message/GraphicMessage.h"

#include "ColibriGui/ColibriWindow.h"
#include <functional>

class GraphicEngine;

/// @brief This class is used to be extended and allow
/// custom operation on a gui element
class CustomGuiToolkit
{
public:
	CustomGuiToolkit() {}
	virtual ~CustomGuiToolkit() = 0;
};

class CustomGuiMessage : public GraphicMessage
{
public:
	CustomGuiMessage(std::function<void(CustomGuiToolkit*, GraphicEngine *)> const &func_p, CustomGuiToolkit * toolkit_p);

	virtual void visit(GraphicMessageHandler &handler_p);

	std::function<void(CustomGuiToolkit*, GraphicEngine *)> const &getFunc() const { return _func; }
	CustomGuiToolkit * getToolkit() const { return _toolkit; }

protected:
	std::function<void(CustomGuiToolkit*, GraphicEngine *)> _func;
	CustomGuiToolkit * const _toolkit;
};

class DestroyWindowMessage : public GraphicMessage
{
public:
	DestroyWindowMessage(Colibri::Window * window_p);

	virtual void visit(GraphicMessageHandler &handler_p);

	Colibri::Window * getWindow() const { return _window; }

protected:
	Colibri::Window * const _window;
};
