#include "BlocModel.h"

BlocModel::BlocModel(std::vector<BlocForm> const &form_p, std::string const &material_p)
	:_forms(form_p), _material(material_p)
{}

std::string const &BlocModel::getMaterial() const
{
	return _material;
}
BlocForm const &BlocModel::getForm(size_t idx_p) const
{
	return _forms.at(idx_p);
}
std::vector<BlocForm> const &BlocModel::getForms() const
{
	return _forms;
}
