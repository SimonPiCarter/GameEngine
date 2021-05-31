#pragma once

#include <array>
#include <list>
#include <unordered_map>

struct Tile
{
	long const x;
	long const y;
	bool const constructible;
};

///
/// @brief this class represent a map layout
/// - tile information
/// - where mob spanw
/// - where mob damage player
/// - the check points the mob needs to go through
///
class MapLayout
{
public:
	MapLayout(std::list<Tile> const &tile_p
		, std::array<long, 2> const &spawnPoint_p
		, std::array<long, 2> const &targetPoint_p
		, std::list<std::array<long, 2> > const &checkPoints_p);

	std::list<Tile> const &getTile() const { return _tile; }
	std::array<long, 2> const &getSpawnPoint() const { return _spawnPoint; }
	std::array<long, 2> const &getTargetPoint() const { return _targetPoint; }
	std::list<std::array<long, 2> > const &getCheckPoints() const { return _checkPoints; }

	Tile const & getTile(long x, long y) const;

private:
	std::list<Tile> _tile;
	std::array<long, 2> _spawnPoint;
	std::array<long, 2> _targetPoint;
	std::list<std::array<long, 2> > _checkPoints;

	std::unordered_map<long, Tile*> _map;
};
