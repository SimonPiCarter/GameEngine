#include "GraphicEntity.h"

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
