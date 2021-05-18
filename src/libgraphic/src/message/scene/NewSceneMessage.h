#pragma once

#include "message/GraphicMessage.h"

#include <array>
#include <string>

class NewSceneMessage : public GraphicMessage
{
public:
	///
	/// @brief create
	/// @param id_p the scene to create
	/// @param parent_p the parent scene
	/// @param pos_p the relative position to the parent
	NewSceneMessage(std::string const &id_p, std::string const &parent_p, const std::array<double, 3> & pos_p);

	virtual void visit(GraphicMessageHandler &handler_p);

	std::string const &getId() const { return _id; }
	std::string const &getParent() const { return _parent; }
	const std::array<double, 3> &getPosition() const { return _position; }

private:
	std::string const _id;
	std::string const _parent;
	const std::array<double, 3> _position;
};
