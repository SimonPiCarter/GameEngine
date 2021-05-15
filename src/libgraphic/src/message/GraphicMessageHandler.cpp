#include "GraphicMessageHandler.h"
#include "engine/GraphicEngine.h"
#include "resource/ResourceLoader.h"

#include "OgreSceneManager.h"
#include "OgreItem.h"

#include <iostream>

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


void GraphicMessageHandler::visitNewGraphicEntity(NewGraphicEntityMessage const &msg_p)
{
	Ogre::SceneManager *sceneManager = _engine->getSceneManager();

	Resource const &res_l = _engine->getResourceHandler()->getResource(msg_p.getResourceId());

	Ogre::MeshPtr v2Mesh_l = loadMesh(res_l);

	//Create an Item with the model we just imported.
	//Notice we use the name of the imported model. We could also use the overload
	//with the mesh pointer:
	Ogre::Item *item = sceneManager->createItem( v2Mesh_l, Ogre::SCENE_DYNAMIC );

	Ogre::SceneNode * rootNode_l = _engine->getMapSceneNode().at(msg_p.getScene());
	Ogre::SceneNode *sceneNode = rootNode_l->
		createChildSceneNode( Ogre::SCENE_DYNAMIC, Ogre::Vector3(msg_p.getPosition()[0], msg_p.getPosition()[1], msg_p.getPosition()[2]) );
	sceneNode->attachObject( item );
	sceneNode->scale(msg_p.getScale()[0], msg_p.getScale()[1], msg_p.getScale()[2]);

}
