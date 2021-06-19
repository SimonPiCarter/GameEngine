#pragma once

#include "message/GraphicMessage.h"

#include <string>
#include <array>


class GraphicEntity;

class DestroyGraphicEntityMessage : public GraphicMessage
{
public:
	DestroyGraphicEntityMessage(GraphicEntity * entity_p, bool delete_p=false);

	virtual void visit(GraphicMessageHandler &handler_p);

	GraphicEntity *getEntity() const { return _entity; }
	bool isDelete() const { return _delete; }

private:
	GraphicEntity * const _entity;
	bool const _delete;
};
