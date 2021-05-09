#pragma once

#include <message/MessageHandler.h>

#include "GraphicMessage.h"

class GraphicMessageHandler : public MessageHandler<GraphicMessage>
{
public:
	GraphicMessageHandler();
	~GraphicMessageHandler();

	// visit methods
	// NoOp (used in test)
	void visitNoOp(NoOpGraphicMessage &msg_p);
protected:
	/// @brief method to be extended to handle message
	virtual void handleMessage(GraphicMessage *msg_p) override;
};
