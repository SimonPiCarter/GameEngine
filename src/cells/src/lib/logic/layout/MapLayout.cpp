#include "MapLayout.h"

#include <cassert>

MapLayout::MapLayout(std::list<Tile> const &tile_p
	, std::array<double, 2> const &spawnPoint_p
	, std::array<double, 2> const &targetPoint_p
	, std::list<std::array<double, 2> > const &checkPoints_p)
	: _tile(tile_p)
	, _spawnPoint(spawnPoint_p)
	, _targetPoint(targetPoint_p)
	, _checkPoints(checkPoints_p)
{
	long maxX_l = 0;
	long maxY_l = 0;
	// Fill map
	for(Tile & tile_l : _tile)
	{
		assert(!_map[tile_l.x* 1e6 + tile_l.y]);
		_map[tile_l.x* 1e6 + tile_l.y] = &tile_l;
		maxX_l = std::max(maxX_l, tile_l.x);
		maxY_l = std::max(maxY_l, tile_l.y);
	}

	_size = {maxX_l, maxY_l};

	for(long i = 0 ; i < _size[0] ; ++ i)
		for(long j = 0 ; j < _size[1] ; ++ j)
			// throw if not filled
			getTile(i , j);
}

Tile const & MapLayout::getTile(long x, long y) const
{
	return *_map.at(x *1e6+ y);
}
