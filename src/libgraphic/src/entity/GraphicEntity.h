#pragma once

#include "Animation/OgreSkeletonAnimation.h"
#include "OgreUserObjectBindings.h"

#include <map>

namespace Ogre
{
	class Item;
	class Light;
	class ParticleSystem;
}

class GraphicEntity
{
public:
	GraphicEntity();

	Ogre::Item * getItem() const;
	void setItem(Ogre::Item * item_p);

	Ogre::Light * getLight() const;
	void setLight(Ogre::Light * light_p);

	Ogre::ParticleSystem * getParticle() const;
	void setParticle(Ogre::ParticleSystem * particle_p);

	Ogre::SkeletonAnimation *getAnimationState(std::string const &animState_p);

	std::map<std::string, Ogre::Any> const & getData() const;

	template<typename data_t>
	void addData(std::string const &key_p, data_t data_p)
	{
		_data[key_p] = Ogre::Any(data_p);
	}

protected:
	Ogre::Item * _item;
	Ogre::Light * _light;
	Ogre::ParticleSystem * _particle;
	/// @brief custom data
	std::map<std::string, Ogre::Any> _data;
};
