#include "MapLayout.h"

#include <cassert>

MapLayout::MapLayout(std::list<Tile> const &tile_p
	, std::array<long, 2> const &spawnPoint_p
	, std::array<long, 2> const &targetPoint_p
	, std::list<std::array<long, 2> > const &checkPoints_p)
	: _tile(tile_p)
	, _spawnPoint(spawnPoint_p)
	, _targetPoint(targetPoint_p)
	, _checkPoints(checkPoints_p)
{
	// Fill map
	for(Tile & tile_l : _tile)
	{
		assert(!_map[tile_l.x* 1e6 + tile_l.y]);
		_map[tile_l.x* 1e6 + tile_l.y] = &tile_l;
	}
}

Tile const & MapLayout::getTile(long x, long y) const
{
	return *_map.at(x *1e6+ y);
}
