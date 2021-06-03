#include "MapDisplay.h"

#include <engine/GraphicEngine.h>

#include "logic/layout/MapLayout.h"

MapDisplay::MapDisplay(std::string const &scene_p, MapLayout const * layout_p)
	: _scene(scene_p)
	, _layout(layout_p)
{}


MapDisplay * newMapDisplay(std::string const &scene_p, MapLayout const * layout_p, GraphicEngine & graphic_p)
{
	MapDisplay * display_l = new MapDisplay(scene_p, layout_p);

	for(Tile const &tile_l : layout_p->getTile())
	{
		display_l->getEntities().push_back(new GraphicEntity());
		graphic_p.registerMessage(new NewGraphicEntityMessage(display_l->getEntities().back(), tile_l.resource,
			{double(tile_l.x), double(tile_l.y), 0.}, {0.5, 0.5, 0.5}, scene_p));
	}

	return display_l;
}
