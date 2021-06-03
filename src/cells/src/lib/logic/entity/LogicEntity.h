#pragma once

#include <array>
#include <list>

#include "logic/wave/PositionalTree.h"

class GraphicEntity;

template<typename T>
class LogicEntity
{
public:
	LogicEntity(std::array<double, 2> const &position_p, std::array<double, 2> const &size_p);
	~LogicEntity();

	std::array<double, 2> const &getPosition() const;
	void setPosition(std::array<double, 2> const &position_p);
	std::array<double, 2> const &getSize() const;
	std::list<StorageInfo<T> > const &getPositionNodes() const;
	std::list<StorageInfo<T> > &getPositionNodes();

	GraphicEntity * getGraphic() { return _graphic; }
	void setGraphic(GraphicEntity * graphic_p) { _graphic = graphic_p; }

protected:
	std::array<double, 2> _position;
	std::array<double, 2> _size;

	std::list<StorageInfo<T> > _storageInfo;

	GraphicEntity * _graphic;
};
