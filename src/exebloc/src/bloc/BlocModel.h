#pragma once

#include <vector>
#include "BlocForm.h"

class BlocModel
{
public:
	BlocModel(std::vector<BlocForm> const &form_p, std::string const &material_p);

	std::string const &getMaterial() const;

	BlocForm const &getForm(size_t idx_p) const;
	std::vector<BlocForm> const &getForms() const;
protected:
	std::vector<BlocForm> const _forms;
	std::string const _material;
};
