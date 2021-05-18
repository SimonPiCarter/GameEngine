#include "GraphicEntity.h"

#include "Animation/OgreSkeletonInstance.h"
#include "OgreItem.h"

GraphicEntity::GraphicEntity()
	: _item(nullptr)
	, _light(nullptr)
{}

Ogre::Item * GraphicEntity::getItem() const
{
	return _item;
}

void GraphicEntity::setItem(Ogre::Item * item_p)
{
	_item = item_p;
}

Ogre::Light * GraphicEntity::getLight() const
{
	return _light;
}

void GraphicEntity::setLight(Ogre::Light * light_p)
{
	_light = light_p;
}

Ogre::SkeletonAnimation *GraphicEntity::getAnimationState(std::string const &animState_p)
{
	Ogre::SkeletonInstance* skeletonInstance_l = _item->getSkeletonInstance();
	assert(skeletonInstance_l);
	return skeletonInstance_l->getAnimation(animState_p);;
}
