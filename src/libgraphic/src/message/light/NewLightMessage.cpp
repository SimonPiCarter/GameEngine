#include "NewLightMessage.h"

#include "message/GraphicMessageHandler.h"

NewLightMessage::NewLightMessage(GraphicEntity * entity_p, std::string const &parent_p, const std::array<double, 3> & pos_p,
	const std::array<double, 3> & dir_p , LightType type_p, double power_p, bool shadows_p, const std::array<double, 3> & diffuse_p
		, const std::array<double, 3> & specular_p , const std::array<double, 2> & attenuation_p )
	: GraphicMessage("")
	, _entity(entity_p)
	, _parent(parent_p)
	, _position(pos_p)
	, _direction(dir_p)
	, _type(type_p)
	, _power(power_p)
	, _shadows(shadows_p)
	, _diffuse(diffuse_p)
	, _specular(specular_p)
	, _attenuation(attenuation_p)
{}

void NewLightMessage::visit(GraphicMessageHandler &handler_p)
{
	handler_p.visitNewLight(*this);
}
