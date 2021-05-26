#pragma once

#include "message/GraphicMessage.h"

#include <string>

class DestroySceneMessage : public GraphicMessage
{
public:
	///
	/// @brief create
	/// @param id_p the scene to destroy
	DestroySceneMessage(std::string const &id_p, bool destroyChildren_p=true);

	virtual void visit(GraphicMessageHandler &handler_p);

	std::string const &getId() const { return _id; }

	bool getDestroyChildren() const { return _destroyChildren; }

private:
	std::string const _id;
	bool const _destroyChildren;
};
