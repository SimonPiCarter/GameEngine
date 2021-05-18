#pragma once

#include "message/GraphicMessage.h"

#include <string>
#include <array>

class GraphicEntity;

class MoveLightMessage : public GraphicMessage
{
public:
	///
	/// @brief create
	/// @param id_p the scene to move
	/// @param vector_p the vector with the coordinate to translate the scene
	MoveLightMessage(GraphicEntity * entity_p, std::array<double, 3> const &vector_p);

	virtual void visit(GraphicMessageHandler &handler_p);

	GraphicEntity *getEntity() const { return _entity; }
	const std::array<double, 3> &getVector() const { return _vector; }

private:
	GraphicEntity * const _entity;
	const std::array<double, 3> _vector;
};
