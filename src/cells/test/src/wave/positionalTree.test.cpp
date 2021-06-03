#include <gtest/gtest.h>

#include "logic/entity/LogicEntityDefs.h"
#include "logic/wave/PositionalTreeDefs.h"

class TestLogicEntity : public LogicEntity<TestLogicEntity>
{
public :
	TestLogicEntity(std::array<double, 2> const &position_p, std::array<double, 2> const &size_p)
		: LogicEntity<TestLogicEntity>(position_p, size_p) {}
};

TEST(posititionalTree, construction_simple_100)
{
	PositionalTree<TestLogicEntity> tree_l({ {0,0}, {100, 100}}, 100);

	ASSERT_EQ(100u, tree_l.getMap().size());
	for(size_t i = 0 ; i < tree_l.getMap().size(); ++ i)
	{
		ASSERT_EQ(100u, tree_l.getMap().at(i).size());
		for(size_t j = 0 ; j < tree_l.getMap().at(i).size(); ++ j)
		{
			ASSERT_TRUE(tree_l.getMap().at(i).at(j));
			EXPECT_NEAR(i, tree_l.getMap().at(i).at(j)->getBox().position[0], 1e-5);
			EXPECT_NEAR(j, tree_l.getMap().at(i).at(j)->getBox().position[1], 1e-5);
			EXPECT_NEAR(1, tree_l.getMap().at(i).at(j)->getBox().size[0], 1e-5);
			EXPECT_NEAR(1, tree_l.getMap().at(i).at(j)->getBox().size[1], 1e-5);
		}
	}
}

TEST(posititionalTree, construction_simple_25)
{
	PositionalTree<TestLogicEntity> tree_l({ {0,0}, {100, 100}}, 25);

	ASSERT_EQ(25u, tree_l.getMap().size());
	for(size_t i = 0 ; i < tree_l.getMap().size(); ++ i)
	{
		ASSERT_EQ(25u, tree_l.getMap().at(i).size());
		for(size_t j = 0 ; j < tree_l.getMap().at(i).size(); ++ j)
		{
			ASSERT_TRUE(tree_l.getMap().at(i).at(j));
			EXPECT_NEAR(i*4, tree_l.getMap().at(i).at(j)->getBox().position[0], 1e-5);
			EXPECT_NEAR(j*4, tree_l.getMap().at(i).at(j)->getBox().position[1], 1e-5);
			EXPECT_NEAR(4, tree_l.getMap().at(i).at(j)->getBox().size[0], 1e-5);
			EXPECT_NEAR(4, tree_l.getMap().at(i).at(j)->getBox().size[1], 1e-5);
		}
	}
}

TEST(posititionalTree, construction_simple_3_links)
{
	PositionalTree<TestLogicEntity> tree_l({ {0,0}, {100, 100}}, 3);

	// top left
	PositionalNode<TestLogicEntity> * node_l = tree_l.getMap().at(0).at(0);
	ASSERT_TRUE(node_l);
	EXPECT_EQ(nullptr, node_l->getUp());
	EXPECT_EQ(nullptr, node_l->getLeft());
	ASSERT_NE(nullptr, node_l->getDown());
	ASSERT_NE(nullptr, node_l->getRight());
	EXPECT_EQ(node_l->getDown()->getUp(), node_l);
	EXPECT_EQ(node_l->getRight()->getLeft(), node_l);

	// central node
	node_l = tree_l.getMap().at(1).at(1);
	ASSERT_TRUE(node_l);
	ASSERT_NE(nullptr, node_l->getUp());
	ASSERT_NE(nullptr, node_l->getLeft());
	ASSERT_NE(nullptr, node_l->getDown());
	ASSERT_NE(nullptr, node_l->getRight());
	EXPECT_EQ(node_l->getUp()->getDown(), node_l);
	EXPECT_EQ(node_l->getLeft()->getRight(), node_l);
	EXPECT_EQ(node_l->getDown()->getUp(), node_l);
	EXPECT_EQ(node_l->getRight()->getLeft(), node_l);
}

TEST(posititionalTree, addContent)
{
	PositionalTree<TestLogicEntity> tree_l({ {0,0}, {100, 100}}, 3);

	// should be added in 0, 0
	TestLogicEntity entity_l({25., 25.}, {5., 5.});
	tree_l.addContent(&entity_l);

	PositionalNode<TestLogicEntity> * node_l = tree_l.getMap().at(0).at(0);

	ASSERT_EQ(1u, node_l->getContent().size());
	EXPECT_EQ(&entity_l, node_l->getContent().at(0));

	// should be added in 1, 2
	TestLogicEntity entity2_l({36.5, 70.5}, {5., 5.});
	tree_l.addContent(&entity2_l);

	node_l = tree_l.getMap().at(1).at(2);

	ASSERT_EQ(1u, node_l->getContent().size());
	EXPECT_EQ(&entity2_l, node_l->getContent().at(0));

	// should be added in 0, 1 and 1, 1
	TestLogicEntity entity3_l({32., 56.}, {5., 5.});
	tree_l.addContent(&entity3_l);

	node_l = tree_l.getMap().at(0).at(1);
	ASSERT_EQ(1u, node_l->getContent().size());
	EXPECT_EQ(&entity3_l, node_l->getContent().at(0));

	node_l = tree_l.getMap().at(1).at(1);
	ASSERT_EQ(1u, node_l->getContent().size());
	EXPECT_EQ(&entity3_l, node_l->getContent().at(0));
}

TEST(posititionalTree, removeContent)
{
	PositionalTree<TestLogicEntity> tree_l({ {0,0}, {100, 100}}, 3);

	// should be added in 0, 0
	TestLogicEntity entity_l({25., 25.}, {5., 5.});
	tree_l.addContent(&entity_l);


	// should be added in 1, 2
	TestLogicEntity entity2_l({34., 68.}, {5., 5.});
	tree_l.addContent(&entity2_l);

	// should be added in 0, 1 and 1, 1
	TestLogicEntity entity3_l({32., 56.}, {5., 5.});
	tree_l.addContent(&entity3_l);

	// remove from 0, 0
	tree_l.removeContent(&entity_l);
	PositionalNode<TestLogicEntity> * node_l = tree_l.getMap().at(0).at(0);
	ASSERT_EQ(1u, node_l->getContent().size());
	EXPECT_EQ(nullptr, node_l->getContent().at(0));

	// remove from 1, 2
	tree_l.removeContent(&entity_l);
	node_l = tree_l.getMap().at(0).at(0);
	ASSERT_EQ(1u, node_l->getContent().size());
	EXPECT_EQ(nullptr, node_l->getContent().at(0));
}

TEST(posititionalTree, moveEntity_simple_stay)
{
	PositionalTree<TestLogicEntity> tree_l({ {0,0}, {100, 100}}, 3);

	// added in 0, 0
	TestLogicEntity entity_l({25., 25.}, {5., 5.});
	tree_l.addContent(&entity_l);

	PositionalNode<TestLogicEntity> * node_l = tree_l.getMap().at(0).at(0);
	ASSERT_EQ(1u, node_l->getContent().size());
	EXPECT_EQ(&entity_l, node_l->getContent().at(0));

	tree_l.updatePositionFromNode(entity_l, {12., 12.});

	node_l = tree_l.getMap().at(0).at(0);
	ASSERT_EQ(1u, node_l->getContent().size());
	EXPECT_EQ(&entity_l, node_l->getContent().at(0));
}

TEST(posititionalTree, moveEntity_simple_stay_overlap)
{
	PositionalTree<TestLogicEntity> tree_l({ {0,0}, {100, 100}}, 3);

	// added in 0, 0
	TestLogicEntity entity_l({25., 25.}, {5., 5.});
	tree_l.addContent(&entity_l);

	PositionalNode<TestLogicEntity> * node_l = tree_l.getMap().at(0).at(0);
	ASSERT_EQ(1u, node_l->getContent().size());
	EXPECT_EQ(&entity_l, node_l->getContent().at(0));

	tree_l.updatePositionFromNode(entity_l, {32.5, 25.});
	entity_l.setPosition({32.5, 25.});

	// still in 0, 0
	node_l = tree_l.getMap().at(0).at(0);
	ASSERT_EQ(1u, node_l->getContent().size());
	EXPECT_EQ(&entity_l, node_l->getContent().at(0));

	// new in 1, 0
	node_l = tree_l.getMap().at(1).at(0);
	ASSERT_EQ(1u, node_l->getContent().size());
	EXPECT_EQ(&entity_l, node_l->getContent().at(0));

	tree_l.updatePositionFromNode(entity_l, {42.5, 25.});
	entity_l.setPosition({42.5, 25.});

	// not in 0, 0
	node_l = tree_l.getMap().at(0).at(0);
	ASSERT_EQ(1u, node_l->getContent().size());
	EXPECT_EQ(nullptr, node_l->getContent().at(0));

	// new in 1, 0
	node_l = tree_l.getMap().at(1).at(0);
	ASSERT_EQ(1u, node_l->getContent().size());
	EXPECT_EQ(&entity_l, node_l->getContent().at(0));
}

TEST(posititionalTree, moveEntity_complex_stay_overlap)
{
	PositionalTree<TestLogicEntity> tree_l({ {0,0}, {100, 100}}, 3);

	// added in 0, 0
	TestLogicEntity entity_l({25., 25.}, {5., 5.});
	tree_l.addContent(&entity_l);

	PositionalNode<TestLogicEntity> * node_l = tree_l.getMap().at(0).at(0);
	ASSERT_EQ(1u, node_l->getContent().size());
	EXPECT_EQ(&entity_l, node_l->getContent().at(0));

	tree_l.updatePositionFromNode(entity_l, {32.5, 32.5});

	// still in 0, 0
	node_l = tree_l.getMap().at(0).at(0);
	ASSERT_EQ(1u, node_l->getContent().size());
	EXPECT_EQ(&entity_l, node_l->getContent().at(0));

	// new in 1, 0
	node_l = tree_l.getMap().at(1).at(0);
	ASSERT_EQ(1u, node_l->getContent().size());
	EXPECT_EQ(&entity_l, node_l->getContent().at(0));

	// new in 0, 1
	node_l = tree_l.getMap().at(0).at(1);
	ASSERT_EQ(1u, node_l->getContent().size());
	EXPECT_EQ(&entity_l, node_l->getContent().at(0));

	// new in 1, 1
	node_l = tree_l.getMap().at(1).at(1);
	ASSERT_EQ(1u, node_l->getContent().size());
	EXPECT_EQ(&entity_l, node_l->getContent().at(0));
}

TEST(posititionalTree, moveEntity_complex_skip_one)
{
	PositionalTree<TestLogicEntity> tree_l({ {0,0}, {100, 100}}, 3);

	// added in 0, 0
	TestLogicEntity entity_l({25., 25.}, {5., 5.});
	tree_l.addContent(&entity_l);

	PositionalNode<TestLogicEntity> * node_l = tree_l.getMap().at(0).at(0);
	ASSERT_EQ(1u, node_l->getContent().size());
	EXPECT_EQ(&entity_l, node_l->getContent().at(0));

	tree_l.updatePositionFromNode(entity_l, {80., 30.});

	// not in 0, 0
	node_l = tree_l.getMap().at(0).at(0);
	ASSERT_EQ(1u, node_l->getContent().size());
	EXPECT_EQ(nullptr, node_l->getContent().at(0));

	// not in 1, 0
	node_l = tree_l.getMap().at(1).at(0);
	ASSERT_EQ(1u, node_l->getContent().size());
	EXPECT_EQ(nullptr, node_l->getContent().at(0));

	// new in 2, 0
	node_l = tree_l.getMap().at(2).at(0);
	ASSERT_EQ(1u, node_l->getContent().size());
	EXPECT_EQ(&entity_l, node_l->getContent().at(0));
}

TEST(posititionalTree, getClosest_simple)
{
	PositionalTree<TestLogicEntity> tree_l({ {0,0}, {100, 100}}, 3);

	// added in 0, 0
	TestLogicEntity entity_l({25., 25.}, {5., 5.});
	tree_l.addContent(&entity_l);
	// added in 0, 0
	TestLogicEntity entity1_l({25., 32.}, {5., 5.});
	tree_l.addContent(&entity1_l);
	// added in 1, 0
	TestLogicEntity entity2_l({52., 32.}, {5., 5.});
	tree_l.addContent(&entity2_l);

	TestLogicEntity * closest_l = tree_l.getClosestFromPosition({23., 25.}, 50);

	EXPECT_EQ(&entity_l, closest_l);
}

TEST(posititionalTree, getClosest_complex_notInNode)
{
	PositionalTree<TestLogicEntity> tree_l({ {0,0}, {100, 100}}, 3);

	// added in 0, 0
	TestLogicEntity entity_l({25., 25.}, {5., 5.});
	tree_l.addContent(&entity_l);
	// added in 0, 0
	TestLogicEntity entity1_l({25., 32.}, {5., 5.});
	tree_l.addContent(&entity1_l);
	// added in 1, 0
	TestLogicEntity entity2_l({52., 32.}, {5., 5.});
	tree_l.addContent(&entity2_l);

	TestLogicEntity * closest_l = tree_l.getClosestFromPosition({67., 82.}, 150);

	EXPECT_EQ(&entity2_l, closest_l);
}

TEST(posititionalTree, getClosest_complex_10k)
{
	PositionalTree<TestLogicEntity> tree_l({ {0,0}, {100, 100}}, 3);

	std::list<TestLogicEntity *> list_l;
	// added in 0, 0
	for(size_t i = 0 ; i < 1e4 ; ++ i )
	{
		list_l.push_back(new TestLogicEntity({25. + (double(i)/1e4), 25.}, {5., 5.}));
		tree_l.addContent(list_l.back());
	}

	TestLogicEntity * closest_l = tree_l.getClosestFromPosition({78., 82.}, 150);

	EXPECT_EQ(list_l.back(), closest_l);
}

TEST(posititionalTree, getAllWithinRadius_simple)
{
	PositionalTree<TestLogicEntity> tree_l({ {0,0}, {100, 100}}, 3);

	// added in 0, 0
	TestLogicEntity entity_l({25., 25.}, {5., 5.});
	tree_l.addContent(&entity_l);
	// added in 0, 0
	TestLogicEntity entity1_l({25., 32.}, {5., 5.});
	tree_l.addContent(&entity1_l);
	// added in 1, 0
	TestLogicEntity entity2_l({52., 32.}, {5., 5.});
	tree_l.addContent(&entity2_l);

	std::unordered_set<TestLogicEntity *> inRadius_l = tree_l.getAllWithinRadius({23., 25.}, 50);

	EXPECT_EQ(3u, inRadius_l.size());
}

TEST(posititionalTree, getAllWithinRadius_simple_not_all)
{
	PositionalTree<TestLogicEntity> tree_l({ {0,0}, {100, 100}}, 3);

	// added in 0, 0
	TestLogicEntity entity_l({25., 25.}, {5., 5.});
	tree_l.addContent(&entity_l);
	// added in 0, 0
	TestLogicEntity entity1_l({25., 32.}, {5., 5.});
	tree_l.addContent(&entity1_l);
	// added in 1, 0
	TestLogicEntity entity2_l({52., 32.}, {5., 5.});
	tree_l.addContent(&entity2_l);

	std::unordered_set<TestLogicEntity *> inRadius_l = tree_l.getAllWithinRadius({23., 25.}, 5.);

	EXPECT_EQ(1u, inRadius_l.size());
}

TEST(posititionalTree, getAllWithinRadius_complex_10k)
{
	PositionalTree<TestLogicEntity> tree_l({ {0,0}, {100, 100}}, 3);

	std::list<TestLogicEntity *> list_l;
	// added in 0, 0
	for(size_t i = 0 ; i < 1e4 ; ++ i )
	{
		list_l.push_back(new TestLogicEntity({25. + (double(i)/1e4), 25.}, {5., 5.}));
		tree_l.addContent(list_l.back());
	}

	std::unordered_set<TestLogicEntity *> inRadius_l = tree_l.getAllWithinRadius({23., 25.}, 5.);

	EXPECT_EQ(1e4, inRadius_l.size());
}

TEST(posititionalTree, getAllWithinLine_simple)
{
	PositionalTree<TestLogicEntity> tree_l({{0,0}, {10, 10}}, 10);

	tree_l.addContent(new TestLogicEntity({6.,4.}, {2.,2.}));

	std::unordered_set<TestLogicEntity *> inLine_l;

	inLine_l = tree_l.getAllWithinLine({1., 2.}, {2.,4.}, 100.);
	EXPECT_EQ(0u, inLine_l.size());

	inLine_l = tree_l.getAllWithinLine({1., 0.}, {2.,1.}, 5.);
	EXPECT_EQ(0u, inLine_l.size());

	inLine_l = tree_l.getAllWithinLine({1., 0.}, {2.,1.}, 6.);
	EXPECT_EQ(1u, inLine_l.size());

	inLine_l = tree_l.getAllWithinLine({9., 2.}, {-2.,1.}, 2.);
	EXPECT_EQ(0u, inLine_l.size());

	inLine_l = tree_l.getAllWithinLine({9., 2.}, {7.,3.}, 3.);
	EXPECT_EQ(1u, inLine_l.size());

	// check direction along x and y only

	inLine_l = tree_l.getAllWithinLine({6., 0.}, {6.,1.}, 1.);
	EXPECT_EQ(0u, inLine_l.size());

	inLine_l = tree_l.getAllWithinLine({6., 0.}, {6.,1.}, 3.);
	EXPECT_EQ(1u, inLine_l.size());

	inLine_l = tree_l.getAllWithinLine({0., 4.}, {1.,4.}, 4.9);
	EXPECT_EQ(0u, inLine_l.size());

	inLine_l = tree_l.getAllWithinLine({0., 4.}, {1.,4.}, 5.);
	EXPECT_EQ(1u, inLine_l.size());

	inLine_l = tree_l.getAllWithinLine({0., 4.}, {1.,4.}, 10.);
	EXPECT_EQ(1u, inLine_l.size());
}
