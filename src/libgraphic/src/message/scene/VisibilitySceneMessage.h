#pragma once

#include "message/GraphicMessage.h"

#include <string>

class VisibilitySceneMessage : public GraphicMessage
{
public:
	///
	/// @brief create
	/// @param id_p the scene to change visibility
	/// @param visibility_p visibility to set
	VisibilitySceneMessage(std::string const &id_p, bool visibility_p);

	virtual void visit(GraphicMessageHandler &handler_p);

	std::string const &getId() const { return _id; }
	bool getVisibility() const { return _visibility; }

private:
	std::string const _id;
	bool const _visibility;
};
