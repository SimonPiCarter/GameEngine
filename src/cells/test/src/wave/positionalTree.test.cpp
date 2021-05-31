#include <gtest/gtest.h>

#include "logic/entity/LogicEntity.h"
#include "logic/wave/PositionalTreeDefs.h"

TEST(posititionalTree, construction_simple_100)
{
	PositionalTree<LogicEntity> tree_l({ {0,0}, {100, 100}}, 100);

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
	PositionalTree<LogicEntity> tree_l({ {0,0}, {100, 100}}, 25);

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
	PositionalTree<LogicEntity> tree_l({ {0,0}, {100, 100}}, 3);

	// top left
	PositionalNode<LogicEntity> * node_l = tree_l.getMap().at(0).at(0);
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
	PositionalTree<LogicEntity> tree_l({ {0,0}, {100, 100}}, 3);

	// should be added in 0, 0
	LogicEntity entity_l({25., 25.}, {5., 5.});
	tree_l.addContent(&entity_l);

	PositionalNode<LogicEntity> * node_l = tree_l.getMap().at(0).at(0);

	ASSERT_EQ(1u, node_l->getContent().size());
	EXPECT_EQ(&entity_l, node_l->getContent().at(0));

	// should be added in 1, 2
	LogicEntity entity2_l({34., 68.}, {5., 5.});
	tree_l.addContent(&entity2_l);

	node_l = tree_l.getMap().at(1).at(2);

	ASSERT_EQ(1u, node_l->getContent().size());
	EXPECT_EQ(&entity2_l, node_l->getContent().at(0));

	// should be added in 0, 1 and 1, 1
	LogicEntity entity3_l({32., 56.}, {5., 5.});
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
	PositionalTree<LogicEntity> tree_l({ {0,0}, {100, 100}}, 3);

	// should be added in 0, 0
	LogicEntity entity_l({25., 25.}, {5., 5.});
	tree_l.addContent(&entity_l);


	// should be added in 1, 2
	LogicEntity entity2_l({34., 68.}, {5., 5.});
	tree_l.addContent(&entity2_l);

	// should be added in 0, 1 and 1, 1
	LogicEntity entity3_l({32., 56.}, {5., 5.});
	tree_l.addContent(&entity3_l);

	// remove from 0, 0
	tree_l.removeContent(&entity_l);
	PositionalNode<LogicEntity> * node_l = tree_l.getMap().at(0).at(0);
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
	PositionalTree<LogicEntity> tree_l({ {0,0}, {100, 100}}, 3);

	// added in 0, 0
	LogicEntity entity_l({25., 25.}, {5., 5.});
	tree_l.addContent(&entity_l);

	PositionalNode<LogicEntity> * node_l = tree_l.getMap().at(0).at(0);
	ASSERT_EQ(1u, node_l->getContent().size());
	EXPECT_EQ(&entity_l, node_l->getContent().at(0));

	tree_l.updatePositionFromNode(entity_l, {12., 12.});

	node_l = tree_l.getMap().at(0).at(0);
	ASSERT_EQ(1u, node_l->getContent().size());
	EXPECT_EQ(&entity_l, node_l->getContent().at(0));
}

TEST(posititionalTree, moveEntity_simple_stay_overlap)
{
	PositionalTree<LogicEntity> tree_l({ {0,0}, {100, 100}}, 3);

	// added in 0, 0
	LogicEntity entity_l({25., 25.}, {5., 5.});
	tree_l.addContent(&entity_l);

	PositionalNode<LogicEntity> * node_l = tree_l.getMap().at(0).at(0);
	ASSERT_EQ(1u, node_l->getContent().size());
	EXPECT_EQ(&entity_l, node_l->getContent().at(0));

	tree_l.updatePositionFromNode(entity_l, {30., 25.});

	// still in 0, 0
	node_l = tree_l.getMap().at(0).at(0);
	ASSERT_EQ(1u, node_l->getContent().size());
	EXPECT_EQ(&entity_l, node_l->getContent().at(0));

	// new in 1, 0
	node_l = tree_l.getMap().at(1).at(0);
	ASSERT_EQ(1u, node_l->getContent().size());
	EXPECT_EQ(&entity_l, node_l->getContent().at(0));
}

TEST(posititionalTree, moveEntity_complex_stay_overlap)
{
	PositionalTree<LogicEntity> tree_l({ {0,0}, {100, 100}}, 3);

	// added in 0, 0
	LogicEntity entity_l({25., 25.}, {5., 5.});
	tree_l.addContent(&entity_l);

	PositionalNode<LogicEntity> * node_l = tree_l.getMap().at(0).at(0);
	ASSERT_EQ(1u, node_l->getContent().size());
	EXPECT_EQ(&entity_l, node_l->getContent().at(0));

	tree_l.updatePositionFromNode(entity_l, {30., 30.});

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
	PositionalTree<LogicEntity> tree_l({ {0,0}, {100, 100}}, 3);

	// added in 0, 0
	LogicEntity entity_l({25., 25.}, {5., 5.});
	tree_l.addContent(&entity_l);

	PositionalNode<LogicEntity> * node_l = tree_l.getMap().at(0).at(0);
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
	PositionalTree<LogicEntity> tree_l({ {0,0}, {100, 100}}, 3);

	// added in 0, 0
	LogicEntity entity_l({25., 25.}, {5., 5.});
	tree_l.addContent(&entity_l);
	// added in 0, 0
	LogicEntity entity1_l({25., 32.}, {5., 5.});
	tree_l.addContent(&entity1_l);
	// added in 1, 0
	LogicEntity entity2_l({52., 32.}, {5., 5.});
	tree_l.addContent(&entity2_l);

	LogicEntity * closest_l = tree_l.getClosestFromPosition({23., 25.}, 50);

	EXPECT_EQ(&entity_l, closest_l);
}

TEST(posititionalTree, getClosest_complex_notInNode)
{
	PositionalTree<LogicEntity> tree_l({ {0,0}, {100, 100}}, 3);

	// added in 0, 0
	LogicEntity entity_l({25., 25.}, {5., 5.});
	tree_l.addContent(&entity_l);
	// added in 0, 0
	LogicEntity entity1_l({25., 32.}, {5., 5.});
	tree_l.addContent(&entity1_l);
	// added in 1, 0
	LogicEntity entity2_l({52., 32.}, {5., 5.});
	tree_l.addContent(&entity2_l);

	LogicEntity * closest_l = tree_l.getClosestFromPosition({67., 82.}, 150);

	EXPECT_EQ(&entity2_l, closest_l);
}

TEST(posititionalTree, getClosest_complex_10k)
{
	PositionalTree<LogicEntity> tree_l({ {0,0}, {100, 100}}, 3);

	std::list<LogicEntity *> list_l;
	// added in 0, 0
	for(size_t i = 0 ; i < 1e4 ; ++ i )
	{
		list_l.push_back(new LogicEntity({25. + (double(i)/1e4), 25.}, {5., 5.}));
		tree_l.addContent(list_l.back());
	}

	LogicEntity * closest_l = tree_l.getClosestFromPosition({78., 82.}, 150);

	EXPECT_EQ(list_l.back(), closest_l);
}

TEST(posititionalTree, getAllWithinRadius_simple)
{
	PositionalTree<LogicEntity> tree_l({ {0,0}, {100, 100}}, 3);

	// added in 0, 0
	LogicEntity entity_l({25., 25.}, {5., 5.});
	tree_l.addContent(&entity_l);
	// added in 0, 0
	LogicEntity entity1_l({25., 32.}, {5., 5.});
	tree_l.addContent(&entity1_l);
	// added in 1, 0
	LogicEntity entity2_l({52., 32.}, {5., 5.});
	tree_l.addContent(&entity2_l);

	std::unordered_set<LogicEntity *> inRadius_l = tree_l.getAllWithinRadius({23., 25.}, 50);

	EXPECT_EQ(3u, inRadius_l.size());
}

TEST(posititionalTree, getAllWithinRadius_simple_not_all)
{
	PositionalTree<LogicEntity> tree_l({ {0,0}, {100, 100}}, 3);

	// added in 0, 0
	LogicEntity entity_l({25., 25.}, {5., 5.});
	tree_l.addContent(&entity_l);
	// added in 0, 0
	LogicEntity entity1_l({25., 32.}, {5., 5.});
	tree_l.addContent(&entity1_l);
	// added in 1, 0
	LogicEntity entity2_l({52., 32.}, {5., 5.});
	tree_l.addContent(&entity2_l);

	std::unordered_set<LogicEntity *> inRadius_l = tree_l.getAllWithinRadius({23., 25.}, 5.);

	EXPECT_EQ(1u, inRadius_l.size());
}

TEST(posititionalTree, getAllWithinRadius_complex_10k)
{
	PositionalTree<LogicEntity> tree_l({ {0,0}, {100, 100}}, 3);

	std::list<LogicEntity *> list_l;
	// added in 0, 0
	for(size_t i = 0 ; i < 1e4 ; ++ i )
	{
		list_l.push_back(new LogicEntity({25. + (double(i)/1e4), 25.}, {5., 5.}));
		tree_l.addContent(list_l.back());
	}

	std::unordered_set<LogicEntity *> inRadius_l = tree_l.getAllWithinRadius({23., 25.}, 5.);

	EXPECT_EQ(1e4, inRadius_l.size());
}
