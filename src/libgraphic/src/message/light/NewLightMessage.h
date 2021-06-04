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
	/// @param power_p power of the light
	/// @param shadows_p true if we want the light to cast shadows (expensive)
	/// @param diffuse_p the diffuse colour of the light
	/// @param specular_p the specular colour of the light
	/// @param attenuation_p the attenuation (radius, threshold), 0 as radius for no attenuation
	NewLightMessage(GraphicEntity * entity_p, std::string const &parent_p, const std::array<double, 3> & pos_p, const std::array<double, 3> & dir_p
		, LightType type_p, double power_p = 3.14, bool shadows_p = false, const std::array<double, 3> & diffuse_p = {1.,1.,1.}
		, const std::array<double, 3> & specular_p = {1.,1.,1.}, const std::array<double, 2> & attenuation_p = {0.,0.});

	virtual void visit(GraphicMessageHandler &handler_p);

	GraphicEntity *getEntity() const { return _entity; }
	std::string const &getParent() const { return _parent; }
	const std::array<double, 3> &getPosition() const { return _position; }
	const std::array<double, 3> &getDirection() const { return _direction; }
	LightType const &getType() const { return _type; }
	double getPower() const { return _power; }
	bool getShadows() const { return _shadows; }
	const std::array<double, 3> &getDiffuse() const { return _diffuse; }
	const std::array<double, 3> &getSpecular() const { return _specular; }
	const std::array<double, 2> &getAttenuation() const { return _attenuation; }

private:
	GraphicEntity * const _entity;
	std::string const _parent;
	const std::array<double, 3> _position;
	const std::array<double, 3> _direction;
	LightType const _type;
	bool const _shadows;
	double const _power;
	const std::array<double, 3> _diffuse;
	const std::array<double, 3> _specular;
	const std::array<double, 2> _attenuation;
};
