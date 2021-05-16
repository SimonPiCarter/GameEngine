#include "Resource.h"

#include <exception>

void ResourceHandler::addResource(Resource const &res_p)
{
	_mapResources[res_p._id] = res_p;
}
Resource const & ResourceHandler::getResource(std::string const id_p) const
{
	if(_mapResources.find(id_p) != _mapResources.end())
	{
		return _mapResources.at(id_p);
	}
	throw std::logic_error("Cannot find resource "+id_p);
}