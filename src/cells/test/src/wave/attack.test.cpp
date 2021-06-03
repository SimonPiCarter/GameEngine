#include <gtest/gtest.h>

#include "logic/LogicEngine.h"
#include "logic/WaveEngine.h"
#include "logic/layout/MapLayout.h"
#include "logic/layout/WaveLayout.h"
#include "logic/entity/Tower.h"

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
/// - mob spawn every second on point 5.5, 5.5 (middle of node 5,5), 3 spawns
/// - they move to point 7.5, 5.5 (middle of node 7,5) then 7.5, 7.5 (middle of node 7,7)
/// - speed of 1 per second
/// - a tower is place on 6.5, 6.5 with a range of 1 and reload time of 1
/// - tower will attack mob when it is on 6.5, 5.5
///
/// we check that the mob has damage
///
TEST(attack, simple_no_reload)
{
	MapLayout mapLayout_l(buildTiles(20)
		, {5.5,5.5}							// spawn position
		, {7.5,7.5}							// despawn position
		, {{7.5,5.5}, {7.5,7.5}});			// check points
	WaveLayout waveLayout_l;
	waveLayout_l.mobLayout.push_back(
		{
			{12., 1., ArmorType::Standard, "", {0.1,0.1}},		// Mob model
			3,												// number of spawn
			2.												// interval
		}
	);

	LogicEngine logic_l(&mapLayout_l);
	WaveEngine wave_l(logic_l);

	// add tower
	logic_l.getTowers().push_back(new Tower({6.5,6.5}, {1., 1.}));

	// prevent wave  engine from looping automatically
	logic_l.quit();

	wave_l.waveLoop(waveLayout_l);

	// 0.9 sec elapsed -> 1 spawn, one 6.4,5.5 : no attack yet
	wave_l.handleFrame(0.9);
	ASSERT_EQ(1u, wave_l.getTree().getMap().at(6).at(5)->getContentSize());
	MobEntity * entity_l = wave_l.getTree().getMap().at(6).at(5)->getContent(0);
	ASSERT_NE(nullptr, entity_l);
	EXPECT_NEAR(12., entity_l->getHitpoint(), 1e-5);

	// 0.1 sec elapsed -> 1 spawn, one 6.5,5.5 : 1 attack yet
	wave_l.handleFrame(0.1);
	ASSERT_EQ(1u, wave_l.getTree().getMap().at(6).at(5)->getContentSize());
	entity_l = wave_l.getTree().getMap().at(6).at(5)->getContent(0);
	ASSERT_NE(nullptr, entity_l);
	EXPECT_NEAR(11., entity_l->getHitpoint(), 1e-5);
}

///
/// build a simple test where
/// - mob spawn every second on point 5.5, 5.5 (middle of node 5,5), 3 spawns
/// - they move to point 7.5, 5.5 (middle of node 7,5) then 7.5, 7.5 (middle of node 7,7)
/// - speed of 1 per second
/// - a tower is place on 6.5, 6.5 with a range of 10 and reload time of 1
/// - tower will attack mob when it is on 6.5, 5.5
///
/// we check that the mob has damage
///
TEST(attack, simple_one_reload)
{
	MapLayout mapLayout_l(buildTiles(20)
		, {5.5,5.5}							// spawn position
		, {7.5,7.5}							// despawn position
		, {{7.5,5.5}, {7.5,7.5}});			// check points
	WaveLayout waveLayout_l;
	waveLayout_l.mobLayout.push_back(
		{
			{12., 1., ArmorType::Standard, "", {0.1,0.1}},		// Mob model
			3,												// number of spawn
			2.												// interval
		}
	);

	LogicEngine logic_l(&mapLayout_l);
	WaveEngine wave_l(logic_l);

	// add tower
	logic_l.getTowers().push_back(new Tower({6.5,6.5}, {1., 1.}));
	logic_l.getTowers().back()->setAttackModifier(AttackModifier(1., 1., 10., 0., AttackType::Direct, DamageType::Standard));

	// prevent wave  engine from looping automatically
	logic_l.quit();

	wave_l.waveLoop(waveLayout_l);

	// 0.9 sec elapsed -> 1 spawn, one 6.4,5.5 : 1 attack yet
	wave_l.handleFrame(0.9);
	ASSERT_EQ(1u, wave_l.getTree().getMap().at(6).at(5)->getContentSize());
	MobEntity * entity_l = wave_l.getTree().getMap().at(6).at(5)->getContent(0);
	ASSERT_NE(nullptr, entity_l);
	EXPECT_NEAR(11., entity_l->getHitpoint(), 1e-5);

	// 0.1 sec elapsed -> 1 spawn, one 6.5,5.5 : 2 attack yet
	wave_l.handleFrame(0.1);
	ASSERT_EQ(1u, wave_l.getTree().getMap().at(6).at(5)->getContentSize());
	entity_l = wave_l.getTree().getMap().at(6).at(5)->getContent(0);
	ASSERT_NE(nullptr, entity_l);
	EXPECT_NEAR(10., entity_l->getHitpoint(), 1e-5);
}

///
/// build a simple test where
/// - mob spawn every second on point 5.5, 5.5 (middle of node 5,5), 2 spawns
/// - they move to point 7.5, 5.5 (middle of node 7,5) then 7.5, 7.5 (middle of node 7,7)
/// - speed of 1 per second
/// - a tower is place on 6.5, 6.5 with a range of 1 and reload time of 1
/// - tower will attack mob when it is on 6.5, 5.5 : arc should go to second mob
///
/// we check that the mob has damage
///
TEST(attack, simple_arc)
{
	MapLayout mapLayout_l(buildTiles(20)
		, {5.5,5.5}							// spawn position
		, {7.5,7.5}							// despawn position
		, {{7.5,5.5}, {7.5,7.5}});			// check points
	WaveLayout waveLayout_l;
	waveLayout_l.mobLayout.push_back(
		{
			{12., 1., ArmorType::Standard, "", {0.1,0.1}},		// Mob model
			2,												// number of spawn
			1.												// interval
		}
	);

	LogicEngine logic_l(&mapLayout_l);
	WaveEngine wave_l(logic_l);

	// add tower
	logic_l.getTowers().push_back(new Tower({6.5,6.5}, {1., 1.}));
	logic_l.getTowers().back()->setAttackModifier(AttackModifier(1., 1., 1., 2., AttackType::Arc, DamageType::Standard));

	// prevent wave  engine from looping automatically
	logic_l.quit();

	wave_l.waveLoop(waveLayout_l);

	// 0.9 sec elapsed -> 1 spawn, one 6.4,5.5 : no attack yet
	wave_l.handleFrame(0.9);
	ASSERT_EQ(1u, wave_l.getTree().getMap().at(6).at(5)->getContentSize());

	MobEntity * entity_l = wave_l.getTree().getMap().at(6).at(5)->getContent(0);
	ASSERT_NE(nullptr, entity_l);
	EXPECT_NEAR(12., entity_l->getHitpoint(), 1e-5);

	// 0.1 sec elapsed -> 2 spawn, one 5.5,5.5; one 6.5,5.5 : 1 attack yet (second attack should arc to second mob)
	wave_l.handleFrame(0.1);
	ASSERT_EQ(1u, wave_l.getTree().getMap().at(5).at(5)->getContentSize());
	ASSERT_EQ(1u, wave_l.getTree().getMap().at(6).at(5)->getContentSize());

	entity_l = wave_l.getTree().getMap().at(6).at(5)->getContent(0);
	ASSERT_NE(nullptr, entity_l);
	EXPECT_NEAR(11., entity_l->getHitpoint(), 1e-5);

	entity_l = wave_l.getTree().getMap().at(5).at(5)->getContent(0);
	ASSERT_NE(nullptr, entity_l);
	EXPECT_NEAR(11., entity_l->getHitpoint(), 1e-5);
}

///
/// build a simple test where
/// - mob spawn every second on point 5.5, 5.5 (middle of node 5,5), 3 spawns
/// - they move to point 7.5, 5.5 (middle of node 7,5) then 7.5, 7.5 (middle of node 7,7)
/// - speed of 1 per second
/// - a tower is place on 6.5, 6.5 with a range of 1 and reload time of 1
/// - tower will attack mob when it is on 6.5, 5.5 : splash should find all mob
///
/// we check that the mob has damage
///
TEST(attack, simple_splash)
{
	MapLayout mapLayout_l(buildTiles(20)
		, {5.5,5.5}							// spawn position
		, {7.5,7.5}							// despawn position
		, {{7.5,5.5}, {7.5,7.5}});			// check points
	WaveLayout waveLayout_l;
	waveLayout_l.mobLayout.push_back(
		{
			{12., 1., ArmorType::Standard, "", {0.1,0.1}},		// Mob model
			3,												// number of spawn
			0.1												// interval
		}
	);

	LogicEngine logic_l(&mapLayout_l);
	WaveEngine wave_l(logic_l);

	// add tower
	logic_l.getTowers().push_back(new Tower({6.5,6.5}, {1., 1.}));
	logic_l.getTowers().back()->setAttackModifier(AttackModifier(1., 1., 1., 1., AttackType::Splash, DamageType::Standard));

	// prevent wave  engine from looping automatically
	logic_l.quit();

	wave_l.waveLoop(waveLayout_l);

	// 0.9 sec elapsed -> 3 spawn, one 6.4,5.5 one 6.3,5.5 one 6.2,5.5 : no attack yet
	wave_l.handleFrame(0.9);
	ASSERT_EQ(3u, wave_l.getTree().getMap().at(6).at(5)->getContentSize());

	// check no attack occured
	MobEntity * entity_l = nullptr;
	for(size_t i = 0 ; i < 3 ; ++ i)
	{
		entity_l = wave_l.getTree().getMap().at(6).at(5)->getContent().at(i);
		ASSERT_NE(nullptr, entity_l);
		EXPECT_NEAR(12., entity_l->getHitpoint(), 1e-5);
	}

	// 0.1 sec elapsed -> 3 spawn,  one 6.5,5.5 one 6.4,5.5 one 6.3,5.5 : one attack should have splashed on all
	wave_l.handleFrame(0.1);
	ASSERT_EQ(3u, wave_l.getTree().getMap().at(6).at(5)->getContentSize());

	// check attacked occured and splashed every one
	for(size_t i = 0 ; i < 3 ; ++ i)
	{
		entity_l = wave_l.getTree().getMap().at(6).at(5)->getContent().at(i);
		ASSERT_NE(nullptr, entity_l);
		EXPECT_NEAR(11., entity_l->getHitpoint(), 1e-5);
	}
}

///
/// build a simple test where
/// - mob spawn every second on point 5.5, 5.5 (middle of node 5,5), 3 spawns
/// - they move to point 7.5, 5.5 (middle of node 7,5) then 7.5, 7.5 (middle of node 7,7)
/// - speed of 1 per second
/// - a tower is place on 8.5, 6.5 with a range of 2 and reload time of 1
/// - tower will attack mob when it is on 6.5, 5.5 : line should find only one mob
/// - tower will attack mob when it is on 5.5, 5.5 : line should find only two mob
///
/// we check that the mob has damage
///
TEST(attack, simple_line)
{
	MapLayout mapLayout_l(buildTiles(20)
		, {5.5,5.5}							// spawn position
		, {7.5,7.5}							// despawn position
		, {{7.5,5.5}, {7.5,7.5}});			// check points
	WaveLayout waveLayout_l;
	waveLayout_l.mobLayout.push_back(
		{
			{12., 1., ArmorType::Standard, "", {0.1,0.1}},		// Mob model
			3,												// number of spawn
			1.												// interval
		}
	);

	LogicEngine logic_l(&mapLayout_l);
	WaveEngine wave_l(logic_l);

	// add tower
	logic_l.getTowers().push_back(new Tower({8.5,5.5}, {1., 1.}));
	logic_l.getTowers().back()->setAttackModifier(AttackModifier(1., 1., 2., 1., AttackType::Line, DamageType::Standard));

	// prevent wave  engine from looping automatically
	logic_l.quit();

	wave_l.waveLoop(waveLayout_l);

	// 0.9 sec elapsed -> 1 spawn, one 6.4,5.5 : no attack yet
	wave_l.handleFrame(0.9);
	ASSERT_EQ(1u, wave_l.getTree().getMap().at(6).at(5)->getContentSize());

	MobEntity * entity_l = nullptr;

	// check no attack occured
	entity_l = wave_l.getTree().getMap().at(6).at(5)->getContent(0);
	ASSERT_NE(nullptr, entity_l);
	EXPECT_NEAR(12., entity_l->getHitpoint(), 1e-5);

	// 0.1 sec elapsed -> 2 spawn,  one 6.5,5.5 one 5.5,5.5 : one attack should have touched only one
	wave_l.handleFrame(0.1);
	ASSERT_EQ(1u, wave_l.getTree().getMap().at(6).at(5)->getContentSize());
	ASSERT_EQ(1u, wave_l.getTree().getMap().at(5).at(5)->getContentSize());

	// check one attack occured
	entity_l = wave_l.getTree().getMap().at(6).at(5)->getContent(0);
	ASSERT_NE(nullptr, entity_l);
	EXPECT_NEAR(11., entity_l->getHitpoint(), 1e-5);

	entity_l = wave_l.getTree().getMap().at(5).at(5)->getContent(0);
	ASSERT_NE(nullptr, entity_l);
	EXPECT_NEAR(12., entity_l->getHitpoint(), 1e-5);

	// 1 sec elapsed -> 3 spawn,  one 7.5,5.5 one 6.5,5.5 one 5.5,5.5 : second attack should have touched two firsts
	wave_l.handleFrame(1.);
	ASSERT_EQ(1u, wave_l.getTree().getMap().at(7).at(5)->getContentSize());
	ASSERT_EQ(1u, wave_l.getTree().getMap().at(6).at(5)->getContentSize());
	ASSERT_EQ(1u, wave_l.getTree().getMap().at(5).at(5)->getContentSize());

	// check second attack occured
	entity_l = wave_l.getTree().getMap().at(7).at(5)->getContent(0);
	ASSERT_NE(nullptr, entity_l);
	EXPECT_NEAR(10., entity_l->getHitpoint(), 1e-5);

	entity_l = wave_l.getTree().getMap().at(6).at(5)->getContent(0);
	ASSERT_NE(nullptr, entity_l);
	EXPECT_NEAR(11., entity_l->getHitpoint(), 1e-5);

	entity_l = wave_l.getTree().getMap().at(5).at(5)->getContent(0);
	ASSERT_NE(nullptr, entity_l);
	EXPECT_NEAR(12., entity_l->getHitpoint(), 1e-5);
}
