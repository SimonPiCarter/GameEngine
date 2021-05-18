#pragma once

#include "Animation/OgreSkeletonAnimation.h"

namespace Ogre
{
	class Item;
}

class GraphicEntity
{
public:
	GraphicEntity();

	Ogre::Item * getItem() const;
	void setItem(Ogre::Item * item_p);

	Ogre::SkeletonAnimation *getAnimationState(std::string const &animState_p);

protected:
	Ogre::Item * _item;
};
