#pragma once

#include "message/GraphicMessage.h"

#include <string>
#include <array>


class GraphicEntity;

class DestroyGraphicEntityMessage : public GraphicMessage
{
public:
	DestroyGraphicEntityMessage(GraphicEntity * entity_p);

	virtual void visit(GraphicMessageHandler &handler_p);

	GraphicEntity *getEntity() const { return _entity; }

private:
	GraphicEntity * const _entity;
};
