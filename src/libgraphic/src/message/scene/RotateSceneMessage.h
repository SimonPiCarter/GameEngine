#pragma once

#include "message/GraphicMessage.h"

#include <string>
#include <array>

class RotateSceneMessage : public GraphicMessage
{
public:
	///
	/// @brief create
	/// @param id_p the scene to move
	/// @param vector_p the vector with the coordinate to translate the scene
	RotateSceneMessage(std::string const &id_p, std::array<double, 3> const &vector_p);

	virtual void visit(GraphicMessageHandler &handler_p);

	std::string const &getId() const { return _id; }
	const std::array<double, 3> &getVector() const { return _vector; }

private:
	std::string const _id;
	const std::array<double, 3> _vector;
};
