#pragma once

#include <message/MessageHandler.h>

#include "GraphicMessage.h"
#include "implem/NewGraphicEntityMessage.h"

class GraphicEngine;

class GraphicMessageHandler : public MessageHandler<GraphicMessage>
{
public:
	GraphicMessageHandler(GraphicEngine *engine_p);
	~GraphicMessageHandler();

	// visit methods
	// NoOp (used in test)
	void visitNoOp(NoOpGraphicMessage const &msg_p);

	void visitNewGraphicEntity(NewGraphicEntityMessage const &msg_p);
protected:
	/// @brief method to be extended to handle message
	virtual void handleMessage(GraphicMessage *msg_p) override;

	GraphicEngine * const _engine;
};
