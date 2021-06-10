#pragma once

#include <message/MessageHandler.h>

#include "GraphicMessage.h"
// camera
#include "camera/LookAtCameraMessage.h"
#include "camera/MoveCameraMessage.h"
#include "camera/RotateCameraMessage.h"
// entity
#include "entity/AnimateGraphicEntityMessage.h"
#include "entity/DestroyGraphicEntityMessage.h"
#include "entity/LinkGraphicEntityMessage.h"
#include "entity/MoveGraphicEntityMessage.h"
#include "entity/NewGraphicEntityMessage.h"
#include "entity/RotateGraphicEntityMessage.h"
// gui
#include "gui/CentralMenuMessage.h"
#include "gui/CustomGuiMessage.h"
#include "gui/RichLabelMessage.h"
// light
#include "light/DestroyLightMessage.h"
#include "light/MoveLightMessage.h"
#include "light/NewLightMessage.h"
#include "light/RotateLightMessage.h"
// particle
#include "particle/DestroyParticleMessage.h"
#include "particle/NewParticleMessage.h"
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
	void visitLookAtCamera(LookAtCameraMessage const &msg_p);
	void visitMoveCamera(MoveCameraMessage const &msg_p);
	void visitRotateCamera(RotateCameraMessage const &msg_p);

	// Entity
	void visitAnimateGraphicEntity(AnimateGraphicEntityMessage const &msg_p);
	void visitDestroyGraphicEntity(DestroyGraphicEntityMessage const &msg_p);
	void visitLinkGraphicEntity(LinkGraphicEntityMessage const &msg_p);
	void visitMoveGraphicEntity(MoveGraphicEntityMessage const &msg_p);
	void visitNewGraphicEntity(NewGraphicEntityMessage const &msg_p);
	void visitRotateGraphicEntity(RotateGraphicEntityMessage const &msg_p);

	// Gui
	void visitCustomGui(CustomGuiMessage const &msg_p);
	void visitNewCentralMenu(NewCentralMenuMessage const &msg_p);
	void visitHideCentralMenu(HideCentralMenuMessage const &msg_p);
	void visitNewRichLabel(NewRichLabelMessage const &msg_p);
	void visitHideRichLabel(HideRichLabelMessage const &msg_p);
	void visitUpdateTextRichLabel(UpdateTextRichLabelMessage const &msg_p);
	void visitSetPositionRichLabel(SetPositionRichLabelMessage const &msg_p);

	// Light
	void visitDestroyLight(DestroyLightMessage const &msg_p);
	void visitMoveLight(MoveLightMessage const &msg_p);
	void visitNewLight(NewLightMessage const &msg_p);
	void visitRotateLight(RotateLightMessage const &msg_p);

	// Particle
	void visitDestroyParticle(DestroyParticleMessage const &msg_p);
	void visitNewParticle(NewParticleMessage const &msg_p);

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
