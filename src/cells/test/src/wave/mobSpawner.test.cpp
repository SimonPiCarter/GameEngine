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

TEST(mobSpawner, simple)
{
	MapLayout mapLayout_l(buildTiles(20), {5.5,5.5}, {15.5,15.5}, {});
	WaveLayout waveLayout_l;
	waveLayout_l.mobLayout.push_back(
		{
			{12., 0., ArmorType::Heavy, "", {0.1,0.1}},
			100,
			5.
		}
	);

	LogicEngine logic_l(&mapLayout_l);
	WaveEngine wave_l(logic_l);

	// prevent wave  engine from looping automatically
	logic_l.quit();

	wave_l.init(waveLayout_l);

	// 2 sec elapsed -> 1 spawn
	wave_l.handleFrame(2.);
	EXPECT_EQ(1u, wave_l.getTree().getMap().at(5).at(5)->getContent().size());

	// 4 sec elapsed -> 1 spawn
	wave_l.handleFrame(2.);
	EXPECT_EQ(1u, wave_l.getTree().getMap().at(5).at(5)->getContent().size());

	// 6 sec elapsed -> 2 spawn (5 sec delay)
	wave_l.handleFrame(2.);
	EXPECT_EQ(2u, wave_l.getTree().getMap().at(5).at(5)->getContent().size());

	// 8 sec elapsed -> 2 spawn
	wave_l.handleFrame(2.);
	EXPECT_EQ(2u, wave_l.getTree().getMap().at(5).at(5)->getContent().size());

	// 10 sec elapsed -> 3 spawn (5 sec delay)
	wave_l.handleFrame(2.);
	EXPECT_EQ(3u, wave_l.getTree().getMap().at(5).at(5)->getContent().size());
}

TEST(mobSpawner, simple_chain)
{
	MapLayout mapLayout_l(buildTiles(20), {5.5,5.5}, {15.5,15.5}, {});
	WaveLayout waveLayout_l;
	// 1 every 2 seconds, 2 spawn
	waveLayout_l.mobLayout.push_back(
		{
			{12., 0., ArmorType::Heavy, "", {0.1,0.1}},
			2,
			2.
		}
	);
	// 1 every second, 2 spawn
	waveLayout_l.mobLayout.push_back(
		{
			{12., 0., ArmorType::Heavy, "", {0.1,0.1}},
			2,
			1.
		}
	);

	LogicEngine logic_l(&mapLayout_l);
	WaveEngine wave_l(logic_l);

	// prevent wave  engine from looping automatically
	logic_l.quit();

	wave_l.init(waveLayout_l);

	// 1 sec elapsed -> 1 spawn
	wave_l.handleFrame(1.);
	EXPECT_EQ(1u, wave_l.getTree().getMap().at(5).at(5)->getContent().size());

	// 2 sec elapsed -> 2 spawn (2sec delay)
	wave_l.handleFrame(1.);
	EXPECT_EQ(2u, wave_l.getTree().getMap().at(5).at(5)->getContent().size());

	// 3 sec elapsed -> 3 spawn (1 sec delay)
	wave_l.handleFrame(1.);
	EXPECT_EQ(3u, wave_l.getTree().getMap().at(5).at(5)->getContent().size());

	// 4 sec elapsed -> 4 spawn (1 sec delay)
	wave_l.handleFrame(1.);
	EXPECT_EQ(4u, wave_l.getTree().getMap().at(5).at(5)->getContent().size());
}
