#pragma once

#include <string>

class GraphicMessageHandler;

class GraphicMessage
{
public:
	GraphicMessage(std::string const &msg_p);
	virtual ~GraphicMessage();

	std::string const & getMsg() const;

	/// @brief visitor method
	virtual void visit(GraphicMessageHandler &handler_p) = 0;

private:
	std::string const _msg;
};

class NoOpGraphicMessage : public GraphicMessage
{
public:
	NoOpGraphicMessage(std::string const &msg_p);

	virtual void visit(GraphicMessageHandler &handler_p);
};
