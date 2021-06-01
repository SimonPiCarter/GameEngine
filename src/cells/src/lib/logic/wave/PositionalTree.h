#pragma once

#include <array>
#include <cmath>
#include <deque>
#include <vector>
#include <unordered_set>
#include <list>
#include <limits>

#include "BoundingBox.h"

template<typename T>
class PositionalNode;

template<typename T>
struct StorageInfo
{
	/// @brief node containing the content
	PositionalNode<T> * node;
	/// @brief index in the node where the content is stored
	unsigned long index;
};

/// @brief PositionalTree allowing access to T content objects
/// T must define
// - std::array<double, 2> const &getPosition() const
// - std::array<double, 2> const &getSize() const
// - std::list<StorageInfo<T> > & getStorageInfo()
template<typename T>
class PositionalTree
{
public:
	/// @brief
	/// @param box_p box of the tree
	/// @param size_t nb of node in each dimension
	PositionalTree(BoundingBox const &box_p, unsigned long size_p);

	/// @brief add a new content to the tree
	void addContent(T * content_p);
	/// @brief removed content from the tree
	void removeContent(T * content_p);

	/// @brief return the list of new position node based on the current ones and the given content
	/// @remark it is important than content_p still has its old bounding box
	void updatePositionFromNode(T & content_p, std::array<double, 2> const &newPos_p);

	/// @brief get the closest content from given position
	T * getClosestFromPosition(std::array<double, 2> const &position_p, double maxRadius_p);

	/// @brief get all content within the given point and radius
	/// @param squared true if the radius is squared
	/// @param ignored_p this node will be excluded from the search
	std::unordered_set<T*> getAllWithinRadius(std::array<double, 2> const & position_p, double radius_p, PositionalNode<T> * ignored_p=nullptr);

	std::vector<std::vector<PositionalNode<T> * > > const &getMap() const { return _map; }
	std::list<PositionalNode<T> > const &getNodes() const { return _nodes; }
	BoundingBox const &getBox() const { return _box; }
	unsigned long const &getSize() const { return _size; }
protected:
	/// @brief return the deepest node intersecting the given box
	std::list<PositionalNode<T> *> getNodeIntersecting(BoundingBox const &box_p);


	/// @brief get the closest content from given position
	template<typename container_t>
	T * getClosestFromPositionDecoy(std::array<double, 2> const &position_p, container_t const &content_p);

	/// @brief get the closest content from given position
	template<typename container_t>
	std::unordered_set<T*> getAllWithinRadiusDecoy(std::array<double, 2> const &position_p, double radius_p, container_t const &content_p);

	std::vector<std::vector<PositionalNode<T> * > > _map;
	std::list<PositionalNode<T> > _nodes;

	BoundingBox const _box;
	unsigned long const _size;
};

template<typename T>
class PositionalNode
{
public:
	PositionalNode(PositionalTree<T> & tree_p, BoundingBox const &box_p)
		: _tree(tree_p)
		, _box(box_p)
		, _left(nullptr)
		, _right(nullptr)
		, _up(nullptr)
		, _down(nullptr)
	{}

	/// @brief return storage index
	unsigned long addContent(T * content_p)
	{
		if(_freeIndexes.empty())
		{
			_content.push_back(content_p);
			return _content.size() - 1;
		} else
		{
			unsigned long index_l = _freeIndexes.front();
			_freeIndexes.pop_front();
			_content[index_l] = content_p;
			return index_l;
		}
	}

	/// @brief remove content
	void removeContent(StorageInfo<T> const &info_p)
	{
		_freeIndexes.push_back(info_p.index);
		_content[info_p.index] = nullptr;
	}

	std::list<unsigned long> const &getFreeIndexes() const { return _freeIndexes; }
	std::deque<T *> const &getContent() const { return _content; }
	BoundingBox const &getBox() const { return _box; }
	PositionalNode<T> * const &getLeft() const { return _left; }
	PositionalNode<T> * const &getRight() const { return _right; }
	PositionalNode<T> * const &getUp() const { return _up; }
	PositionalNode<T> * const &getDown() const { return _down; }


protected:
	/// @brief list of indexes equal to nullptr in the deque _content
	std::list<unsigned long> _freeIndexes;
	/// @brief element in the node (some may be nullptr)
	std::deque<T *> _content;
	/// @brief reference to master tree
	PositionalTree<T> & _tree;
	/// @brief the bounding box of this node
	BoundingBox const _box;

	/// @brief all neighbours (used to update content link when moving)
	PositionalNode<T> * _left;
	PositionalNode<T> * _right;
	PositionalNode<T> * _up;
	PositionalNode<T> * _down;

	// position tree is the owner of this class
	friend class PositionalTree<T>;
};
