#include "LogicEntity.h"

LogicEntity:: LogicEntity(std::array<double, 2> const &position_p, std::array<double, 2> const &size_p)
	: _position(position_p)
	, _size(size_p)
{}

std::array<double, 2> const &LogicEntity::getPosition() const
{
	return _position;
}
void LogicEntity::setPosition(std::array<double, 2> const &position_p)
{
	_position = position_p;
}
std::array<double, 2> const &LogicEntity::getSize() const
{
	return _size;
}
std::list<StorageInfo<LogicEntity> > const &LogicEntity::getPositionNodes() const
{
	return _storageInfo;
}
std::list<StorageInfo<LogicEntity> > &LogicEntity::getPositionNodes()
{
	return _storageInfo;
}
