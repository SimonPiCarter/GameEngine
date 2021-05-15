#pragma once

#include <unordered_map>
#include <string>

struct Resource
{
	std::string _id;
	std::string _meshPath;
	bool _v1Mesh;
};

class ResourceHandler
{
public:
	void addResource(Resource const &res_p);
	Resource const & getResource(std::string const id_p) const;

protected:
	std::unordered_map<std::string, Resource> _mapResources;
};
