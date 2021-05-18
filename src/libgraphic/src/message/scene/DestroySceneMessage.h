#pragma once

#include "message/GraphicMessage.h"

#include <string>

class DestroySceneMessage : public GraphicMessage
{
public:
	///
	/// @brief create
	/// @param id_p the scene to destroy
	DestroySceneMessage(std::string const &id_p);

	virtual void visit(GraphicMessageHandler &handler_p);

	std::string const &getId() const { return _id; }

private:
	std::string const _id;
};
