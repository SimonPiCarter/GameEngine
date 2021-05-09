#pragma once

#include <message/MessageHandler.h>

#include "LogicMessage.h"

class LogicMessageHandler : public MessageHandler<LogicMessage>
{
public:
	LogicMessageHandler();
	~LogicMessageHandler();

	// visit methods
	// NoOp (used in test)
	void visitNoOp(NoOpLogicMessage &msg_p);
protected:
	/// @brief method to be extended to handle message
	virtual void handleMessage(LogicMessage *msg_p) override;
};
