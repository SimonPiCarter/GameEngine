#pragma once

#include "message/GraphicMessage.h"

#include <string>
#include <array>


class GraphicEntity;

class NewParticleMessage : public GraphicMessage
{
public:
	NewParticleMessage(GraphicEntity * entity_p, std::string const &resourceId_p, std::string const &scene_p);

	virtual void visit(GraphicMessageHandler &handler_p);

	GraphicEntity *getEntity() const { return _entity; }
	const std::string &getResourceId() const { return _resourceId; }
	const std::string &getScene() const { return _scene; }

private:
	GraphicEntity * const _entity;
	const std::string _resourceId;
	const std::string _scene;
};
