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

Ogre::ParticleSystem * GraphicEntity::getParticle() const
{
	return _particle;
}

void GraphicEntity::setParticle(Ogre::ParticleSystem * particle_p)
{
	_particle = particle_p;
}

Ogre::SkeletonAnimation *GraphicEntity::getAnimationState(std::string const &animState_p)
{
	assert(_item);
	Ogre::SkeletonInstance* skeletonInstance_l = _item->getSkeletonInstance();
	assert(skeletonInstance_l);
	return skeletonInstance_l->getAnimation(animState_p);;
}

Ogre::UserObjectBindings const & GraphicEntity::getData() const
{
	return _data;
}
Ogre::UserObjectBindings & GraphicEntity::getData()
{
	return _data;
}
