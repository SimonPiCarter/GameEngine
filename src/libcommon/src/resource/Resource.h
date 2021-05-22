#pragma once

#include <unordered_map>
#include <string>

struct Resource
{
	std::string _id;
	std::string _meshPath;
	bool _v1Mesh;
	/// @brief in case we want to overwrite the material of the mesh, leave empty to use the mesh original one
	std::string _material;
};

class ResourceHandler
{
public:
	ResourceHandler(std::string const &rootPath_p) : _rootPath(rootPath_p) {}

	void addResource(Resource const &res_p);
	Resource const & getResource(std::string const id_p) const;

	std::string const &getRootPath() const { return _rootPath; }

protected:
	std::unordered_map<std::string, Resource> _mapResources;

	std::string const _rootPath;
};
