#pragma once

#include <array>
#include <string>

class BlocModel
{
public:
	BlocModel(std::array<std::array<std::array<bool, 3>, 3>, 3> const &form_p, std::string const &material_p);

	std::array<std::array<std::array<bool, 3>, 3>, 3> const &getForm() const;
	std::string const &getMaterial() const;

	/// @brief min bounding box
	std::array<size_t, 3> const &getMinCorner() const;
	/// @brief max bounding box
	std::array<size_t, 3> const &getMaxCorner() const;

protected:
	std::array<std::array<std::array<bool, 3>, 3>, 3> const _form;
	std::string const _material;

	std::array<size_t, 3> _minCorner;
	std::array<size_t, 3> _maxCorner;
};
