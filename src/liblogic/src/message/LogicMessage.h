#pragma once

#include <string>

class LogicMessageHandler;

class LogicMessage
{
public:
	LogicMessage(std::string const &msg_p);
	virtual ~LogicMessage();

	std::string const & getMsg() const;

	/// @brief visitor method
	virtual void visit(LogicMessageHandler &handler_p) = 0;

private:
	std::string const _msg;
};

class NoOpLogicMessage : public LogicMessage
{
public:
	NoOpLogicMessage(std::string const &msg_p);

	virtual void visit(LogicMessageHandler &handler_p);
};
