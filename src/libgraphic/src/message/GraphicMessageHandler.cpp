#include "GraphicMessageHandler.h"
#include "engine/GraphicEngine.h"
#include "resource/ResourceLoader.h"
#include "entity/GraphicEntity.h"

#include "Animation/OgreSkeletonInstance.h"
#include "Compositor/OgreCompositorWorkspace.h"
#include "Compositor/OgreCompositorShadowNode.h"
#include "OgreCamera.h"
#include "OgreItem.h"
#include "OgreNode.h"
#include "OgreMaterialManager.h"
#include "OgreSceneManager.h"
#include "OgreParticleSystem.h"

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
//          Utils               //
//                              //
//////////////////////////////////

namespace
{
void destroyAttachedObject(Ogre::SceneNode * node_l)
{
	// Destroy all the attached objects
	Ogre::SceneNode::ObjectIterator itObject_l = node_l->getAttachedObjectIterator();

	while ( itObject_l.hasMoreElements() )
		node_l->getCreator()->destroyMovableObject(itObject_l.getNext());

	// Recurse to child SceneNodes
	Ogre::SceneNode::NodeVecIterator itChild_l = node_l->getChildIterator();

	while ( itChild_l.hasMoreElements() )
	{
		Ogre::SceneNode* pChildNode_l = static_cast<Ogre::SceneNode*>(itChild_l.getNext());
		destroyAttachedObject(pChildNode_l);
	}
}
}
//////////////////////////////////
//                              //
//          Camera              //
//                              //
//////////////////////////////////

void GraphicMessageHandler::visitLookAtCamera(LookAtCameraMessage const &msg_p)
{
	_engine->getCamera()->lookAt(Ogre::Vector3(msg_p.getVector()[0], msg_p.getVector()[1], msg_p.getVector()[2]));
}

void GraphicMessageHandler::visitMoveCamera(MoveCameraMessage const &msg_p)
{
	_engine->getCamera()->setPosition( Ogre::Vector3(msg_p.getVector()[0], msg_p.getVector()[1], msg_p.getVector()[2]));
}

void GraphicMessageHandler::visitRotateCamera(RotateCameraMessage const &msg_p)
{
	_engine->getCamera()->roll(Ogre::Degree(msg_p.getVector()[2]));
	_engine->getCamera()->pitch(Ogre::Degree(msg_p.getVector()[0]));
	_engine->getCamera()->yaw(Ogre::Degree(msg_p.getVector()[1]));
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

void GraphicMessageHandler::visitDestroyGraphicEntity(DestroyGraphicEntityMessage const &msg_p)
{
	assert(msg_p.getEntity());
	assert(msg_p.getEntity()->getItem());
	// destroy attached object from parent node (which only contains this entity)
	Ogre::SceneNode * node_l = msg_p.getEntity()->getItem()->getParentSceneNode();
	destroyAttachedObject(node_l);
	// destroy node related to this entity
	Ogre::SceneNode * parentNode_l = node_l->getParentSceneNode();
	assert(parentNode_l);
	// Destroy scene and all children
	parentNode_l->removeAndDestroyChild(node_l);
}

void GraphicMessageHandler::visitLinkGraphicEntity(LinkGraphicEntityMessage const &msg_p)
{
	assert(msg_p.getEntity());
	assert(msg_p.getEntity()->getItem());
	// destroy attached object from parent node (which only contains this entity)
	Ogre::SceneNode * node_l = msg_p.getEntity()->getItem()->getParentSceneNode();
	// get parent node if any
	Ogre::SceneNode * parentNode_l = node_l->getParentSceneNode();
	if(parentNode_l)
	{
		parentNode_l->removeChild(node_l);
	}
	Ogre::SceneNode * newParent_l = _engine->getMapSceneNode()[msg_p.getScene()];
	newParent_l->addChild(node_l);
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
	if(!res_l._material.empty())
	{
		item_l->setDatablockOrMaterialName(res_l._material);
	}

	Ogre::SceneNode * rootNode_l = _engine->getMapSceneNode()[msg_p.getScene()];
	assert(rootNode_l);
	Ogre::SceneNode *sceneNode = rootNode_l->
		createChildSceneNode( Ogre::SCENE_DYNAMIC, Ogre::Vector3(msg_p.getPosition()[0], msg_p.getPosition()[1], msg_p.getPosition()[2]) );
	sceneNode->attachObject(item_l);
	sceneNode->scale(res_l._scale, res_l._scale, res_l._scale);

	item_l->getUserObjectBindings() = msg_p.getEntity()->getData();
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
//           Gui                //
//                              //
//////////////////////////////////

void GraphicMessageHandler::visitCustomGui(CustomGuiMessage const &msg_p)
{
	msg_p.getFunc()(msg_p.getToolkit(), _engine);
}

void GraphicMessageHandler::visitDestroyWindow(DestroyWindowMessage const &msg_p)
{
	_engine->getColibriManager()->destroyWindow(msg_p.getWindow());
}

void GraphicMessageHandler::visitNewCentralMenu(NewCentralMenuMessage const &msg_p)
{
	msg_p.getMenu().menu = new central_menu::Menu(msg_p.getTitle(), msg_p.getData(), *_engine);
}

void GraphicMessageHandler::visitHideCentralMenu(HideCentralMenuMessage const &msg_p)
{
	msg_p.getMenu().menu->setHidden(msg_p.isHidden());
}

void GraphicMessageHandler::visitNewRichLabel(NewRichLabelMessage const &msg_p)
{
	// set pointer
	msg_p.getLabel().label = new RichLabel(msg_p.getContent(), msg_p.getX(), msg_p.getY(), msg_p.getWidth(), msg_p.getHeight()
		, msg_p.getSize(), msg_p.getBack(), *_engine);
}

void GraphicMessageHandler::visitHideRichLabel(HideRichLabelMessage const &msg_p)
{
	msg_p.getLabel().label->setHidden(msg_p.isHidden());
}

void GraphicMessageHandler::visitUpdateTextRichLabel(UpdateTextRichLabelMessage const &msg_p)
{
	msg_p.getLabel().label->updateText(msg_p.getContent());
}

void GraphicMessageHandler::visitSetPositionRichLabel(SetPositionRichLabelMessage const &msg_p)
{
	msg_p.getLabel().label->setPosition(msg_p.getX(), msg_p.getY());
}

//////////////////////////////////
//                              //
//           Light              //
//                              //
//////////////////////////////////


void GraphicMessageHandler::visitDestroyLight(DestroyLightMessage const &msg_p)
{
	assert(msg_p.getEntity());
	assert(msg_p.getEntity()->getLight());
	// destroy attached object from parent node (which only contains this entity)
	Ogre::SceneNode * node_l = msg_p.getEntity()->getLight()->getParentSceneNode();
	destroyAttachedObject(node_l);
	// destroy node related to this entity
	Ogre::SceneNode * parentNode_l = node_l->getParentSceneNode();
	assert(parentNode_l);
	// Destroy scene and all children
	parentNode_l->removeAndDestroyChild(node_l);

	// look for available shadow map
	for(size_t i = 0 ; i < _engine->getShadowMaps().max_size() ; ++ i)
	{
		if(_engine->getShadowMaps()[i] == msg_p.getEntity())
		{
			_engine->getShadowMaps()[i] = nullptr;
		}
	}
}

void GraphicMessageHandler::visitMoveLight(MoveLightMessage const &msg_p)
{
	assert(msg_p.getEntity());
	assert(msg_p.getEntity()->getLight());
	assert(msg_p.getEntity()->getLight()->getParentNode());
	msg_p.getEntity()->getLight()->getParentNode()->translate(msg_p.getVector()[0], msg_p.getVector()[1], msg_p.getVector()[2]);
}

void GraphicMessageHandler::visitNewLight(NewLightMessage const &msg_p)
{
	Ogre::SceneManager *sceneManager_l = _engine->getSceneManager();

	Ogre::CompositorWorkspace *workspace_l = _engine->getCompositorWorkspace();
	Ogre::CompositorShadowNode *shadowNode_l = workspace_l->findShadowNode("StaticShadowMapsShadowNode");
	assert(shadowNode_l);

	Ogre::SceneNode * rootNode_l = _engine->getMapSceneNode()[msg_p.getParent()];
	assert(rootNode_l);

	Ogre::Light *light_l = sceneManager_l->createLight();
	Ogre::SceneNode *lightNode = rootNode_l->createChildSceneNode(Ogre::SCENE_DYNAMIC,
		Ogre::Vector3(msg_p.getPosition()[0], msg_p.getPosition()[1], msg_p.getPosition()[2]));
	lightNode->attachObject(light_l);
	light_l->setPowerScale(msg_p.getPower()); //Since we don't do HDR, counter the PBS' division by PI
	light_l->setDirection(Ogre::Vector3(msg_p.getDirection()[0], msg_p.getDirection()[1], msg_p.getDirection()[2]).normalisedCopy());
	light_l->setDiffuseColour(msg_p.getDiffuse()[0], msg_p.getDiffuse()[1], msg_p.getDiffuse()[2]);
	light_l->setSpecularColour(msg_p.getSpecular()[0], msg_p.getSpecular()[1], msg_p.getSpecular()[2]);
	light_l->setAttenuation(5., 0., 1., 2.);
	if(msg_p.getAttenuation()[0] > 1e-5)
	{
		light_l->setAttenuationBasedOnRadius(msg_p.getAttenuation()[0], msg_p.getAttenuation()[1]);
	}

	if(msg_p.getType() == LightType::Directional)
	{
		light_l->setType(Ogre::Light::LT_DIRECTIONAL);
		if(msg_p.getShadows())
		{
			// look for available shadow map
			for(size_t i = 0 ; i < _engine->getShadowMaps().max_size() ; ++ i)
			{
				if(!_engine->getShadowMaps()[i])
				{
					shadowNode_l->setLightFixedToShadowMap(i, light_l);
					_engine->getShadowMaps()[i] = msg_p.getEntity();
					break;
				}
			}
		}
	}
	if(msg_p.getType() == LightType::Point)
	{
		light_l->setType(Ogre::Light::LT_POINT);
	}
	if(msg_p.getType() == LightType::Spotlight)
	{
		light_l->setType(Ogre::Light::LT_SPOTLIGHT);
	}

	msg_p.getEntity()->setLight(light_l);
}

void GraphicMessageHandler::visitRotateLight(RotateLightMessage const &msg_p)
{
	assert(msg_p.getEntity());
	assert(msg_p.getEntity()->getLight());
	assert(msg_p.getEntity()->getLight()->getParentNode());

	msg_p.getEntity()->getLight()->getParentNode()->roll(Ogre::Degree(msg_p.getVector()[2]));
	msg_p.getEntity()->getLight()->getParentNode()->pitch(Ogre::Degree(msg_p.getVector()[0]));
	msg_p.getEntity()->getLight()->getParentNode()->yaw(Ogre::Degree(msg_p.getVector()[1]));
}


//////////////////////////////////
//                              //
//          Particle            //
//                              //
//////////////////////////////////

void GraphicMessageHandler::visitDestroyParticle(DestroyParticleMessage const &msg_p)
{
	assert(msg_p.getEntity());
	assert(msg_p.getEntity()->getParticle());
	// destroy attached object from parent node (which only contains this entity)
	Ogre::SceneNode * node_l = msg_p.getEntity()->getParticle()->getParentSceneNode();
	destroyAttachedObject(node_l);
	// destroy node related to this entity
	Ogre::SceneNode * parentNode_l = node_l->getParentSceneNode();
	assert(parentNode_l);
	// Destroy scene and all children
	parentNode_l->removeAndDestroyChild(node_l);
	delete msg_p.getEntity();
}

void GraphicMessageHandler::visitNewParticle(NewParticleMessage const &msg_p)
{
	// Get scene manager
	Ogre::SceneManager *sceneManager_l = _engine->getSceneManager();

	// Create particle
	Ogre::ParticleSystem* particleSystem_l = sceneManager_l->createParticleSystem(msg_p.getResourceId());
	msg_p.getEntity()->setParticle(particleSystem_l);

	Ogre::SceneNode * rootNode_l = _engine->getMapSceneNode()[msg_p.getScene()];
	assert(rootNode_l);
	Ogre::SceneNode *particleNode = rootNode_l->createChildSceneNode(Ogre::SCENE_DYNAMIC,
		Ogre::Vector3(msg_p.getPosition()[0], msg_p.getPosition()[1], msg_p.getPosition()[2]));

	// attach the particle system to a scene node
	particleNode->attachObject(particleSystem_l);
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

	if(msg_p.getDestroyChildren())
	{
		destroyAttachedObject(sceneNode_l);
		// Destroy scene and all children
		parentNode_l->removeAndDestroyChild(sceneNode_l);
	}
	else
	{
		parentNode_l->removeChild(sceneNode_l);
	}
	// remove from map of scenes
	_engine->getMapSceneNode().erase(_engine->getMapSceneNode().find(msg_p.getId()));
}

void GraphicMessageHandler::visitMoveScene(MoveSceneMessage const &msg_p)
{
	Ogre::SceneNode * sceneNode_l = _engine->getMapSceneNode()[msg_p.getId()];
	assert(sceneNode_l);
	if(msg_p.isTranslate())
	{
		sceneNode_l->translate(msg_p.getVector()[0], msg_p.getVector()[1], msg_p.getVector()[2]);
	} else
	{
		sceneNode_l->setPosition(msg_p.getVector()[0], msg_p.getVector()[1], msg_p.getVector()[2]);
	}
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

void GraphicMessageHandler::visitVisibilityScene(VisibilitySceneMessage const &msg_p)
{
	Ogre::SceneNode * sceneNode_l = _engine->getMapSceneNode()[msg_p.getId()];
	assert(sceneNode_l);
	sceneNode_l->setVisible(msg_p.getVisibility());
}
