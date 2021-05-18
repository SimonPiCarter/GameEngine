#pragma once

#include "Animation/OgreSkeletonAnimation.h"

namespace Ogre
{
	class Item;
}

struct AnimationState
{
	/// @brief pointer to the ogre animation
	Ogre::SkeletonAnimation * animation = nullptr;
	/// @brief boolean to track if the animation is registered in the GraphicEngine for update on every frame
	bool registered = false;
};

class GraphicEntity
{
public:
	GraphicEntity();

	Ogre::Item * getItem() const;
	void setItem(Ogre::Item * item_p);

	AnimationState &getAnimationState(std::string const &animState_p);

protected:
	Ogre::Item * _item;

	std::unordered_map<std::string, AnimationState> _mapState;
};
