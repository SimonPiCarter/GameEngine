#pragma once

#include "message/GraphicMessage.h"

#include <string>
#include <array>


class GraphicEntity;

class LinkGraphicEntityMessage : public GraphicMessage
{
public:
	LinkGraphicEntityMessage(GraphicEntity * entity_p, std::string const &scene_p);

	virtual void visit(GraphicMessageHandler &handler_p);

	GraphicEntity *getEntity() const { return _entity; }
	const std::string &getScene() const { return _scene; }

private:
	GraphicEntity * const _entity;
	const std::string _scene;
};
