#pragma once

#include "message/GraphicMessage.h"

#include <string>
#include <array>

class GraphicEntity;

class LookAtCameraMessage : public GraphicMessage
{
public:
	///
	/// @brief create
	/// @param vector_p the vector with the coordinate to target the camera
	LookAtCameraMessage(std::array<double, 3> const &vector_p);

	virtual void visit(GraphicMessageHandler &handler_p);

	const std::array<double, 3> &getVector() const { return _vector; }

private:
	const std::array<double, 3> _vector;
};
