#pragma once

#include <array>

///
/// @brief represent a 3x3 bloc with a given shape, a bloc model
/// is composed of N form where each of the form correspond to a rotation
///
class BlocForm
{
public:
	BlocForm(std::array<std::array<std::array<bool, 3>, 3>, 3> const &form_p);

	std::array<std::array<std::array<bool, 3>, 3>, 3> const &getForm() const;

	/// @brief min bounding box
	std::array<size_t, 3> const &getMinCorner() const;
	/// @brief max bounding box
	std::array<size_t, 3> const &getMaxCorner() const;

private:
	std::array<std::array<std::array<bool, 3>, 3>, 3> const _form;

	std::array<size_t, 3> _minCorner;
	std::array<size_t, 3> _maxCorner;
};
