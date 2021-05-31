#pragma once

#include <array>
#include <list>

#include "logic/wave/PositionalTree.h"

template<typename T>
class LogicEntity
{
public:
	LogicEntity(std::array<double, 2> const &position_p, std::array<double, 2> const &size_p);

	std::array<double, 2> const &getPosition() const;
	void setPosition(std::array<double, 2> const &position_p);
	std::array<double, 2> const &getSize() const;
	std::list<StorageInfo<T> > const &getPositionNodes() const;
	std::list<StorageInfo<T> > &getPositionNodes();

protected:
	std::array<double, 2> _position;
	std::array<double, 2> _size;

	std::list<StorageInfo<T> > _storageInfo;
};
