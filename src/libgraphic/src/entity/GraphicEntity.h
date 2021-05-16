#pragma once

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

protected:
	Ogre::Item * _item;
};
