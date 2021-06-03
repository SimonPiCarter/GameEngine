#pragma once

#include "message/GraphicMessage.h"

#include <string>
#include <array>


class GraphicEntity;

class DestroyParticleMessage : public GraphicMessage
{
public:
	DestroyParticleMessage(GraphicEntity * entity_p);

	virtual void visit(GraphicMessageHandler &handler_p);

	GraphicEntity *getEntity() const { return _entity; }

private:
	GraphicEntity * const _entity;
};
