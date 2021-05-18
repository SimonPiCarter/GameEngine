#pragma once

#include "message/GraphicMessage.h"

#include <string>
#include <array>


class GraphicEntity;

class MoveGraphicEntityMessage : public GraphicMessage
{
public:
	///
	/// @brief create
	/// @param entity_p the entity to move
	/// @param vector_p the vector with the coordinate to translate the Entity
	MoveGraphicEntityMessage(GraphicEntity * entity_p, std::array<double, 3> const &vector_p);

	virtual void visit(GraphicMessageHandler &handler_p);

	GraphicEntity *getEntity() const { return _entity; }
	const std::array<double, 3> &getVector() const { return _vector; }

private:
	GraphicEntity * const _entity;
	const std::array<double, 3> _vector;
};
