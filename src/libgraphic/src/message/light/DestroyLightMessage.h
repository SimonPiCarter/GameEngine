#pragma once

#include "message/GraphicMessage.h"

#include <string>

class GraphicEntity;

class DestroyLightMessage : public GraphicMessage
{
public:
	///
	/// @brief create
	/// @param id_p the scene to destroy
	DestroyLightMessage(GraphicEntity * entity_p);

	virtual void visit(GraphicMessageHandler &handler_p);

	GraphicEntity *getEntity() const { return _entity; }

private:
	GraphicEntity * const _entity;
};
