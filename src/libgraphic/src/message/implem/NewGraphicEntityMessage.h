#pragma once

#include "message/GraphicMessage.h"

#include <string>
#include <array>


class GraphicEntity;

class NewGraphicEntityMessage : public GraphicMessage
{
public:
	NewGraphicEntityMessage(GraphicEntity * entity_p, std::string const &resourceId_p, std::array<double, 3> const &pos_p, std::array<double, 3> const &scale_p, std::string const &scene_p);

	virtual void visit(GraphicMessageHandler &handler_p);

	GraphicEntity *getEntity() const { return _entity; }
	const std::string &getResourceId() const { return _resourceId; }
	const std::array<double, 3> &getPosition() const { return _position; }
	const std::array<double, 3> &getScale() const { return _scale; }
	const std::string &getScene() const { return _scene; }

private:
	GraphicEntity * const _entity;
	const std::string _resourceId;
	const std::array<double, 3> _position;
	const std::array<double, 3> _scale;
	const std::string _scene;
};
