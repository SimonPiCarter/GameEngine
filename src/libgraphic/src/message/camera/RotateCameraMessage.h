#pragma once

#include "message/GraphicMessage.h"

#include <string>
#include <array>

class GraphicEntity;

class RotateCameraMessage : public GraphicMessage
{
public:
	///
	/// @brief move the main camera
	/// @param vector_p the vector with the rotation for each angles (in degrees)
	RotateCameraMessage(std::array<double, 3> const &vector_p);

	virtual void visit(GraphicMessageHandler &handler_p);

	const std::array<double, 3> &getVector() const { return _vector; }

private:
	const std::array<double, 3> _vector;
};
