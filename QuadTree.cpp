#include "QuadTree.hpp"

#include <functional>
#include <cassert>


namespace
{
	constexpr auto	MaxLevels	= 5u;
	constexpr auto	MaxObjects	= 2u;
}


QuadTree::QuadTree(std::size_t Level, const sf::FloatRect& Bounds)
	: mObjects()
	, mChildren()
	, mBounds(Bounds)
	, mlevel(Level)
{
}

void QuadTree::clear()
{
	mObjects.clear();

	for (auto& child : mChildren)
		child.reset();
}

void QuadTree::insert(SceneNode* object)
{
	if (hasChildren() && insertInChild(object))
		return;

	mObjects.push_back(object);

	// This node is already split, and we can't move any objects down.
	if (hasChildren())
		return;

	// Can't split this node, so no point checking number of objects.
	if (mlevel == MaxLevels)
		return;

	// Don't need to split this node yet.
	if (mObjects.size() < MaxObjects)
		return;

	split();

	mObjects.erase(
		std::remove_if(mObjects.begin(), mObjects.end(),
			std::bind(&QuadTree::insertInChild, this, std::placeholders::_1)),
		mObjects.end());
}

void QuadTree::getCloseObjects(const sf::FloatRect& Bounds, ObjectsContainer& returnObjects) const
{
	if (hasChildren())
	{
		auto index = getIndex(Bounds);

		if (index != NotFound)
			mChildren[index]->getCloseObjects(Bounds, returnObjects);
	}

	std::copy(mObjects.begin(), mObjects.end(), std::back_inserter(returnObjects));
}

void QuadTree::split()
{
	auto width	= mBounds.width / 2.f;
	auto height = mBounds.height / 2.f;
	auto x		= mBounds.left;
	auto y		= mBounds.top;

	mChildren[TopLeft]		= std::make_unique<QuadTree>(mlevel + 1, sf::FloatRect(x + width, y, width, height));
	mChildren[TopRight]		= std::make_unique<QuadTree>(mlevel + 1, sf::FloatRect(x, y, width, height));
	mChildren[BottomLeft]	= std::make_unique<QuadTree>(mlevel + 1, sf::FloatRect(x, y + height, width, height));
	mChildren[BottomRight]	= std::make_unique<QuadTree>(mlevel + 1, sf::FloatRect(x + width, y + height, width, height));
}

QuadTree::Quadrant QuadTree::getIndex(const sf::FloatRect& Rect) const
{
	assert(Rect.height >= 0.f);
	assert(Rect.width >= 0.f);

	auto index = NotFound;

	auto verticalMidpoint	= mBounds.left + mBounds.width / 2.f;
	auto horizontalMidpoint = mBounds.top + mBounds.height / 2.f;

	// Object can completely fit within the top quadrants
	bool topQuadrant = (Rect.top < horizontalMidpoint && Rect.top + Rect.height < horizontalMidpoint);

	// Object can completely fit within the bottom quadrants
	bool bottomQuadrant = (Rect.top > horizontalMidpoint);

	// Object can completely fit within the left quadrants
	if (Rect.left < verticalMidpoint && Rect.left + Rect.width < verticalMidpoint)
	{
		if (topQuadrant)
		{
			index = TopRight;
		}
		else if (bottomQuadrant)
		{
			index = BottomLeft;
		}
	}

	// Object can completely fit within the right quadrants
	else if (Rect.left > verticalMidpoint)
	{
		if (topQuadrant)
		{
			index = TopLeft;
		}
		else if (bottomQuadrant)
		{
			index = BottomRight;
		}
	}

	return index;
}

bool QuadTree::insertInChild(SceneNode* object) const
{
	assert(hasChildren());

	auto index = getIndex(object->getBoundingRect());

	if (index == NotFound)
		return false;

	mChildren[index]->insert(object);

	return true;
}

bool QuadTree::hasChildren() const
{
	return mChildren[0] != nullptr;
}