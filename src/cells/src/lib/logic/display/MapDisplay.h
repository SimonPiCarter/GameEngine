#pragma once

#include <list>
#include <string>

class MapLayout;
class GraphicEngine;
class GraphicEntity;

class MapDisplay
{
public:
	MapDisplay(std::string const &scene_p, MapLayout const * layout_p);

	std::list<GraphicEntity *> &getEntities() { return _entities; }
	std::list<GraphicEntity *> const &getEntities() const { return _entities; }
	std::string const &getScene() const { return _scene; }
	MapLayout const * getLayout() const { return _layout; }

protected:
	std::list<GraphicEntity *> _entities;
	std::string const _scene;
	MapLayout const * const _layout;
};

MapDisplay * newMapDisplay(std::string const &scene_p, MapLayout const * layout_p, GraphicEngine & engine_p);
