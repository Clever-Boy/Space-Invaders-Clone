#pragma once


#include "SceneNode.hpp"

#include <array>


class QuadTree final : private sf::NonCopyable
{
	static const auto DefaultNodes	= 4u;
	using Ptr						= std::unique_ptr<QuadTree>;
	using ChildrenContainer			= std::array<Ptr, DefaultNodes>;
	using ObjectsContainer			= std::vector<SceneNode*>;


public:
	explicit				QuadTree(std::size_t Level, const sf::FloatRect& Bounds);
							~QuadTree();

	void					clear();
	void					insert(SceneNode& object);
	void					getCloseObjects(const sf::FloatRect& Bounds, ObjectsContainer& returnObjects) const;


private:
	void					split();
	int						getIndex(const sf::FloatRect& Rect) const;


private:
	ObjectsContainer		mObjects;
	ChildrenContainer		mChildren;

	sf::FloatRect			mBounds;
	std::size_t				mlevel;
};
