#pragma once


#include "SceneNode.hpp"

#include <array>


class QuadTree final : private sf::NonCopyable
{
public:
	using Ptr = std::unique_ptr<QuadTree>;


public:
	explicit							QuadTree(std::size_t Level, const sf::FloatRect& Bounds);
										~QuadTree();

	void								clear();
	void								insert(SceneNode& object);
	void								getCloseObjects(const sf::FloatRect& Bounds, std::vector<SceneNode*>& returnObjects);


#ifdef _DEBUG
	void								draw(sf::RenderTarget& target);
#endif


private:
	void								split();
	int									getIndex(const sf::FloatRect& Rect);


private:
	static constexpr auto				DefaultNodes	= 4u;

	sf::FloatRect						mBounds;
	std::size_t							mlevel;

	std::vector<SceneNode*>				mObjects;
	std::array<Ptr, DefaultNodes>		mChildren;
};
