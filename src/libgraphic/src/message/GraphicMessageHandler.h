#pragma once

#include <message/MessageHandler.h>

#include "GraphicMessage.h"
// entity
#include "entity/AnimateGraphicEntityMessage.h"
#include "entity/MoveGraphicEntityMessage.h"
#include "entity/NewGraphicEntityMessage.h"
#include "entity/RotateGraphicEntityMessage.h"
// scene
#include "scene/DestroySceneMessage.h"
#include "scene/MoveSceneMessage.h"
#include "scene/NewSceneMessage.h"
#include "scene/RotateSceneMessage.h"
#include "scene/VisibilitySceneMessage.h"

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

	// Entity
	void visitAnimateGraphicEntity(AnimateGraphicEntityMessage const &msg_p);
	void visitMoveGraphicEntity(MoveGraphicEntityMessage const &msg_p);
	void visitNewGraphicEntity(NewGraphicEntityMessage const &msg_p);
	void visitRotateGraphicEntity(RotateGraphicEntityMessage const &msg_p);

	// Scene
	void visitDestroyScene(DestroySceneMessage const &msg_p);
	void visitMoveScene(MoveSceneMessage const &msg_p);
	void visitNewScene(NewSceneMessage const &msg_p);
	void visitRotateScene(RotateSceneMessage const &msg_p);
	void visitVisibilityScene(VisibilitySceneMessage const &msg_p);

protected:
	/// @brief method to be extended to handle message
	virtual void handleMessage(GraphicMessage *msg_p) override;

	GraphicEngine * const _engine;
};
