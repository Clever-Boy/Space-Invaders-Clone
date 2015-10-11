#pragma once


#include "SceneNode.hpp"

#include <array>


class QuadTree final : public sf::Drawable, private sf::NonCopyable
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
	explicit				QuadTree(std::size_t Level, const sf::FloatRect& Bounds);

	void					clear();
	void					insert(SceneNode* object);
	void					getCloseObjects(const sf::FloatRect& Bounds, ObjectsContainer& returnObjects) const;


private:
	void					draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void					split();
	Quadrant				getIndex(const sf::FloatRect& Rect) const;
	bool					insertInChild(SceneNode* object) const;
	bool					hasChildren() const;


private:
	ObjectsContainer		mObjects;
	ChildrenContainer		mChildren;

	sf::FloatRect			mBounds;
	std::size_t				mLevel;
};
