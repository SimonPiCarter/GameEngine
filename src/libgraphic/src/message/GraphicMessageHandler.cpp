#include "GraphicMessageHandler.h"
#include "engine/GraphicEngine.h"
#include "resource/ResourceLoader.h"
#include "entity/GraphicEntity.h"

#include "OgreSceneManager.h"
#include "OgreNode.h"
#include "Animation/OgreSkeletonInstance.h"
#include "OgreItem.h"

GraphicMessageHandler::GraphicMessageHandler(GraphicEngine *engine_p) : _engine(engine_p) {}
GraphicMessageHandler::~GraphicMessageHandler() {}

void GraphicMessageHandler::handleMessage(GraphicMessage *msg_p)
{
	msg_p->visit(*this);
}

void GraphicMessageHandler::visitNoOp(NoOpGraphicMessage const &)
{
	// NA
}

//////////////////////////////////
//                              //
//          Entity              //
//                              //
//////////////////////////////////


void GraphicMessageHandler::visitAnimateGraphicEntity(AnimateGraphicEntityMessage const &msg_p)
{
	assert(msg_p.getEntity());
	Ogre::SkeletonAnimation * anim_l = msg_p.getEntity()->getAnimationState(msg_p.getAnimation());
	assert(anim_l);
	anim_l->setEnabled(msg_p.isEnable());
	anim_l->setLoop(msg_p.isLoop());
}

void GraphicMessageHandler::visitMoveGraphicEntity(MoveGraphicEntityMessage const &msg_p)
{
	assert(msg_p.getEntity());
	assert(msg_p.getEntity()->getItem());
	assert(msg_p.getEntity()->getItem()->getParentNode());
	msg_p.getEntity()->getItem()->getParentNode()->translate(msg_p.getVector()[0], msg_p.getVector()[1], msg_p.getVector()[2]);
}

void GraphicMessageHandler::visitNewGraphicEntity(NewGraphicEntityMessage const &msg_p)
{
	Ogre::SceneManager *sceneManager = _engine->getSceneManager();

	Resource const &res_l = _engine->getResourceHandler()->getResource(msg_p.getResourceId());

	Ogre::MeshPtr v2Mesh_l = loadMesh(res_l);

	//Create an Item with the model we just imported.
	//Notice we use the name of the imported model. We could also use the overload
	//with the mesh pointer:
	Ogre::Item *item_l = sceneManager->createItem( v2Mesh_l, Ogre::SCENE_DYNAMIC );

	Ogre::SceneNode * rootNode_l = _engine->getMapSceneNode()[msg_p.getScene()];
	assert(rootNode_l);
	Ogre::SceneNode *sceneNode = rootNode_l->
		createChildSceneNode( Ogre::SCENE_DYNAMIC, Ogre::Vector3(msg_p.getPosition()[0], msg_p.getPosition()[1], msg_p.getPosition()[2]) );
	sceneNode->attachObject(item_l);
	sceneNode->scale(msg_p.getScale()[0], msg_p.getScale()[1], msg_p.getScale()[2]);

	msg_p.getEntity()->setItem(item_l);
}

void GraphicMessageHandler::visitRotateGraphicEntity(RotateGraphicEntityMessage const &msg_p)
{
	assert(msg_p.getEntity());
	assert(msg_p.getEntity()->getItem());
	assert(msg_p.getEntity()->getItem()->getParentNode());

	msg_p.getEntity()->getItem()->getParentNode()->roll(Ogre::Degree(msg_p.getVector()[2]));
	msg_p.getEntity()->getItem()->getParentNode()->pitch(Ogre::Degree(msg_p.getVector()[0]));
	msg_p.getEntity()->getItem()->getParentNode()->yaw(Ogre::Degree(msg_p.getVector()[1]));
}

//////////////////////////////////
//                              //
//          Scene               //
//                              //
//////////////////////////////////


void GraphicMessageHandler::visitDestroyScene(DestroySceneMessage const &msg_p)
{
	Ogre::SceneNode * sceneNode_l = _engine->getMapSceneNode()[msg_p.getId()];
	assert(sceneNode_l);
	Ogre::SceneNode * parentNode_l = sceneNode_l->getParentSceneNode();
	assert(parentNode_l);
	// Destroy scene and all children
	parentNode_l->removeAndDestroyChild(sceneNode_l);
	// remove from map of scenes
	_engine->getMapSceneNode().erase(_engine->getMapSceneNode().find(msg_p.getId()));
}

void GraphicMessageHandler::visitMoveScene(MoveSceneMessage const &msg_p)
{
	Ogre::SceneNode * sceneNode_l = _engine->getMapSceneNode()[msg_p.getId()];
	assert(sceneNode_l);
	sceneNode_l->translate(msg_p.getVector()[0], msg_p.getVector()[1], msg_p.getVector()[2]);
}

void GraphicMessageHandler::visitNewScene(NewSceneMessage const &msg_p)
{
	Ogre::SceneNode * rootNode_l = _engine->getMapSceneNode()[msg_p.getParent()];
	assert(rootNode_l);
	Ogre::SceneNode *sceneNode = rootNode_l->createChildSceneNode( Ogre::SCENE_DYNAMIC,
		Ogre::Vector3(msg_p.getPosition()[0], msg_p.getPosition()[1], msg_p.getPosition()[2]) );
	_engine->getMapSceneNode()[msg_p.getId()] = sceneNode;
}

void GraphicMessageHandler::visitRotateScene(RotateSceneMessage const &msg_p)
{
	Ogre::SceneNode * sceneNode_l = _engine->getMapSceneNode()[msg_p.getId()];
	assert(sceneNode_l);

	sceneNode_l->roll(Ogre::Degree(msg_p.getVector()[2]));
	sceneNode_l->pitch(Ogre::Degree(msg_p.getVector()[0]));
	sceneNode_l->yaw(Ogre::Degree(msg_p.getVector()[1]));
}

