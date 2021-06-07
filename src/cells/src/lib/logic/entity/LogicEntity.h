#pragma once

#include <array>
#include <list>

#include "logic/wave/PositionalTree.h"
#include "logic/utils/Hitbox.h"

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

	Hitbox const &getMainHitbox() const { return _mainHitbox; }
	void setMainHitbox(Hitbox const &mainHitbox_p) const { _mainHitbox = mainHitbox_p; }
	Hitbox const &getSecondaryHitbox() const { return _secondaryHitbox; }
	void setSecondaryHitbox(Hitbox const &secondaryHitbox_p) const { _secondaryHitbox = secondaryHitbox_p; }

protected:
	std::array<double, 2> _position;
	std::array<double, 2> _size;

	std::list<StorageInfo<T> > _storageInfo;

	GraphicEntity * _graphic;

	/// @brief hitbox used for mouse interaction
	Hitbox _mainHitbox;
	/// @brief hitbox used for mouse interaction (secondary, used only if no main hitbox has been found)
	Hitbox _secondaryHitbox;
};
