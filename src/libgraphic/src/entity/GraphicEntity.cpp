#include "GraphicEntity.h"

#include "Animation/OgreSkeletonInstance.h"
#include "OgreItem.h"

GraphicEntity::GraphicEntity()
	: _item(nullptr)
{}

Ogre::Item * GraphicEntity::getItem() const
{
	return _item;
}

void GraphicEntity::setItem(Ogre::Item * item_p)
{
	_item = item_p;
}

AnimationState &GraphicEntity::getAnimationState(std::string const &animState_p)
{
	auto it_l = _mapState.find(animState_p);
	if(it_l != _mapState.end())
	{
		return it_l->second;
	}
	Ogre::SkeletonInstance* skeletonInstance_l = _item->getSkeletonInstance();
	assert(skeletonInstance_l);
	AnimationState & state_l = _mapState[animState_p];
	state_l.animation = skeletonInstance_l->getAnimation(animState_p);
	assert(state_l.animation);
	return state_l;
}
