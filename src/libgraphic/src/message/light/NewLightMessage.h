#pragma once

#include "message/GraphicMessage.h"

#include <array>
#include <string>

class GraphicEntity;

enum class LightType
{
	Directional,
	Point,
	Spotlight
};

class NewLightMessage : public GraphicMessage
{
public:
	///
	/// @brief create
	/// @param entity_p the entity to create
	/// @param parent_p the parent scene
	/// @param pos_p the relative position to the parent
	/// @param dir_p the direction of the lighting (if necessary)
	/// @param type_p the type of light
	NewLightMessage(GraphicEntity * entity_p, std::string const &parent_p, const std::array<double, 3> & pos_p, const std::array<double, 3> & dir_p, LightType type_p);

	virtual void visit(GraphicMessageHandler &handler_p);

	GraphicEntity *getEntity() const { return _entity; }
	std::string const &getParent() const { return _parent; }
	const std::array<double, 3> &getPosition() const { return _position; }
	const std::array<double, 3> &getDirection() const { return _direction; }
	LightType const &getType() const { return _type; }

private:
	GraphicEntity * const _entity;
	std::string const _parent;
	const std::array<double, 3> _position;
	const std::array<double, 3> _direction;
	LightType const _type;
};
