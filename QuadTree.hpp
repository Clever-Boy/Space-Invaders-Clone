#pragma once


#include "SceneNode.hpp"

#include <array>


class QuadTree final : public sf::Drawable, private sf::NonCopyable
{
public:
	using Ptr = std::unique_ptr<QuadTree>;


private:
	static constexpr auto DefaultNodes	= 4u;
	using ChildrenContainer				= std::array<Ptr, DefaultNodes>;
	using ObjectsContainer				= std::vector<SceneNode*>;


public:
	explicit				QuadTree(std::size_t Level, const sf::FloatRect& Bounds);
							~QuadTree();

	void					clear();
	void					insert(SceneNode& object);
	void					getCloseObjects(const sf::FloatRect& Bounds, ObjectsContainer& returnObjects);


private:
	void					draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void					split();
	int						getIndex(const sf::FloatRect& Rect);


private:
	ObjectsContainer		mObjects;
	ChildrenContainer		mChildren;

	sf::FloatRect			mBounds;
	std::size_t				mlevel;
};
