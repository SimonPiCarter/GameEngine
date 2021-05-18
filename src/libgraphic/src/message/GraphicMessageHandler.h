#pragma once

#include <message/MessageHandler.h>

#include "GraphicMessage.h"
#include "entity/NewGraphicEntityMessage.h"
#include "entity/MoveGraphicEntityMessage.h"
#include "entity/AnimateGraphicEntityMessage.h"
#include "entity/RotateGraphicEntityMessage.h"

#include <Animation/OgreSkeletonAnimation.h>

class GraphicEngine;

class GraphicMessageHandler : public MessageHandler<GraphicMessage>
{
public:
	GraphicMessageHandler(GraphicEngine *engine_p);
	~GraphicMessageHandler();

	// visit methods
	// NoOp (used in test)
	void visitNoOp(NoOpGraphicMessage const &msg_p);

	void visitAnimateGraphicEntity(AnimateGraphicEntityMessage const &msg_p);
	void visitMoveGraphicEntity(MoveGraphicEntityMessage const &msg_p);
	void visitNewGraphicEntity(NewGraphicEntityMessage const &msg_p);
	void visitRotateGraphicEntity(RotateGraphicEntityMessage const &msg_p);
protected:
	/// @brief method to be extended to handle message
	virtual void handleMessage(GraphicMessage *msg_p) override;

	GraphicEngine * const _engine;
};
