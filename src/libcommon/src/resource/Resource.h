#pragma once

#include <list>
#include <unordered_map>
#include <string>

struct Resource
{
	std::string _id;
	std::string _meshPath;
	bool _v1Mesh;
	/// @brief in case we want to overwrite the material of the mesh, leave empty to use the mesh original one
	std::string _material;

	/// @brief scaling factor
	double _scale;
};

class ResourceHandler
{
public:
	ResourceHandler(std::string const &rootPath_p) : _rootPath(rootPath_p) {}

	void addResource(Resource const &res_p);
	Resource const & getResource(std::string const &id_p) const;

	void addUISkin(std::string const &path_p);
	std::list<std::string> const &getListUISkins() const;

	void addResourcePath(std::string const &path_p);
	std::list<std::string> const &getResourcePaths() const;


	std::string const &getRootPath() const { return _rootPath; }

protected:
	std::unordered_map<std::string, Resource> _mapResources;
	std::list<std::string> _listUISkins;
	std::list<std::string> _listResourcePaths;

	std::string const _rootPath;
};
