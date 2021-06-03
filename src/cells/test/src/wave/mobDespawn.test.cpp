#include <gtest/gtest.h>

#include "logic/LogicEngine.h"
#include "logic/WaveEngine.h"
#include "logic/layout/MapLayout.h"
#include "logic/layout/WaveLayout.h"

namespace{
std::list<Tile> buildTiles(long size_p)
{
	std::list<Tile> list_l;
	for(long i = 0 ; i < size_p ; ++ i)
		for(long j = 0 ; j < size_p ; ++ j)
			list_l.push_back({i, j, false, ""});
	return list_l;
}
}

///
/// build a simple test where
/// - mob spawn every second on point 5.5, 5.5 (middle of node 5,5), 5 spawn
/// - they move to point 7.5, 5.5 (middle of node 7,5) then 7.5, 7.5 (middle of node 7,7)
/// - speed of 1 per second
///
/// this means that every 2 second a new mob is spawned on 5,5 and all others have progress 2 case
///
TEST(mobDespawn, simple_every_2)
{
	MapLayout mapLayout_l(buildTiles(20)
		, {5.5,5.5}							// spawn position
		, {7.5,7.5}							// despawn position
		, {{7.5,5.5}, {7.5,7.5}});			// check points
	WaveLayout waveLayout_l;
	waveLayout_l.mobLayout.push_back(
		{
			{12., 1., ArmorType::Heavy, "", {0.1,0.1}},		// Mob model
			3,												// number of spawn
			2.												// interval
		}
	);

	LogicEngine logic_l(&mapLayout_l);
	WaveEngine wave_l(logic_l);

	// prevent wave  engine from looping automatically
	logic_l.quit();

	wave_l.waveLoop(waveLayout_l);

	// 2. sec elapsed -> 2 spawns, one 5,5 and none 6,5 and one 7,5
	wave_l.handleFrame(2.);
	ASSERT_EQ(1u, wave_l.getTree().getMap().at(5).at(5)->getContentSize());
	ASSERT_EQ(0u, wave_l.getTree().getMap().at(6).at(5)->getContentSize());
	ASSERT_EQ(1u, wave_l.getTree().getMap().at(7).at(5)->getContentSize());

	// 1.5 sec elapsed -> 2 spawns, none 5,5 and one between 6,5 and 7,5 and one between 7,6 and 7,7
	wave_l.handleFrame(1.5);
	ASSERT_EQ(0u, wave_l.getTree().getMap().at(5).at(5)->getContentSize());
	ASSERT_EQ(1u, wave_l.getTree().getMap().at(6).at(5)->getContentSize());
	ASSERT_EQ(1u, wave_l.getTree().getMap().at(7).at(5)->getContentSize());
	ASSERT_EQ(1u, wave_l.getTree().getMap().at(7).at(6)->getContentSize());
	ASSERT_EQ(1u, wave_l.getTree().getMap().at(7).at(7)->getContentSize());

	// 0.5 sec elapsed -> 3 spawns, one 5,5 and none 6,5 and one 7,5 and one on 7,7 but has been despawned
	wave_l.handleFrame(0.5);
	ASSERT_EQ(1u, wave_l.getTree().getMap().at(5).at(5)->getContentSize());
	ASSERT_EQ(0u, wave_l.getTree().getMap().at(6).at(5)->getContentSize());
	ASSERT_EQ(1u, wave_l.getTree().getMap().at(7).at(5)->getContentSize());
	ASSERT_EQ(0u, wave_l.getTree().getMap().at(7).at(6)->getContentSize());
	ASSERT_EQ(0u, wave_l.getTree().getMap().at(7).at(7)->getContentSize());

	// 2 sec elapsed -> 3 spawns, none 5,5 and none 6,5 and one 7,5 and one on 7,7 but has been despawned
	wave_l.handleFrame(2);
	ASSERT_EQ(0u, wave_l.getTree().getMap().at(5).at(5)->getContentSize());
	ASSERT_EQ(0u, wave_l.getTree().getMap().at(6).at(5)->getContentSize());
	ASSERT_EQ(1u, wave_l.getTree().getMap().at(7).at(5)->getContentSize());
	ASSERT_EQ(0u, wave_l.getTree().getMap().at(7).at(6)->getContentSize());
	ASSERT_EQ(0u, wave_l.getTree().getMap().at(7).at(7)->getContentSize());

	// 2 sec elapsed -> 3 spawns, none 5,5 and none 6,5 and none 7,5 and one on 7,7 but has been despawned
	wave_l.handleFrame(2);
	ASSERT_EQ(0u, wave_l.getTree().getMap().at(5).at(5)->getContentSize());
	ASSERT_EQ(0u, wave_l.getTree().getMap().at(6).at(5)->getContentSize());
	ASSERT_EQ(0u, wave_l.getTree().getMap().at(7).at(5)->getContentSize());
	ASSERT_EQ(0u, wave_l.getTree().getMap().at(7).at(6)->getContentSize());
	ASSERT_EQ(0u, wave_l.getTree().getMap().at(7).at(7)->getContentSize());
}
