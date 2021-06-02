#pragma once

#include "LogicEntity.h"

template<typename T>
LogicEntity<T>::LogicEntity(std::array<double, 2> const &position_p, std::array<double, 2> const &size_p)
	: _position(position_p)
	, _size(size_p)
{}

template<typename T>
std::array<double, 2> const &LogicEntity<T>::getPosition() const
{
	return _position;
}
template<typename T>
void LogicEntity<T>::setPosition(std::array<double, 2> const &position_p)
{
	_position = position_p;
}
template<typename T>
std::array<double, 2> const &LogicEntity<T>::getSize() const
{
	return _size;
}
template<typename T>
std::list<StorageInfo<T> > const &LogicEntity<T>::getPositionNodes() const
{
	return _storageInfo;
}
template<typename T>
std::list<StorageInfo<T> > &LogicEntity<T>::getPositionNodes()
{
	return _storageInfo;
}
