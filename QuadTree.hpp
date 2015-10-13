#pragma once


#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/NonCopyable.hpp>

#include <array>
#include <vector>
#include <memory>


class SceneNode;


class QuadTree final : private sf::NonCopyable
{
	enum Quadrant
	{
		NotFound = -1,
		TopLeft, 
		TopRight,
		BottomLeft, 
		BottomRight,
	};

	static const auto DefaultNodes	= 4u;
	using Ptr						= std::unique_ptr<QuadTree>;
	using ChildrenContainer			= std::array<Ptr, DefaultNodes>;
	using ObjectsContainer			= std::vector<SceneNode*>;


public:
	explicit				QuadTree(std::size_t level, const sf::FloatRect& bounds);

	void					clear();
	void					insert(SceneNode* object);
	void					getCloseObjects(const sf::FloatRect& bounds, ObjectsContainer& returnObjects) const;


private:
	void					split();
	Quadrant				getIndex(const sf::FloatRect& bounds) const;
	bool					insertInChild(SceneNode* object) const;
	bool					hasChildren() const;


private:
	ObjectsContainer		mObjects;
	ChildrenContainer		mChildren;

	sf::FloatRect			mBounds;
	std::size_t				mLevel;
};
