#include "QuadTree.hpp"
#include "SceneNode.hpp"

#include <functional>
#include <cassert>


namespace
{
	constexpr auto	MaxLevels	= 5u;
	constexpr auto	MaxObjects	= 2u;
}


QuadTree::QuadTree(std::size_t level, const sf::FloatRect& bounds)
	: mObjects()
	, mChildren()
	, mBounds(bounds)
	, mLevel(level)
{
}

void QuadTree::clear()
{
	mObjects.clear();

	if (!hasChildren())
		return;

	for (auto& child : mChildren)
	{
		child->clear();
		child.reset();
	}
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
	if (mLevel == MaxLevels)
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

void QuadTree::getCloseObjects(const sf::FloatRect& bounds, ObjectsContainer& returnObjects) const
{
	if (hasChildren())
	{
		auto index = getIndex(bounds);

		if (index != NotFound)
			mChildren[index]->getCloseObjects(bounds, returnObjects);
	}

	std::copy(mObjects.begin(), mObjects.end(), std::back_inserter(returnObjects));
}

void QuadTree::split()
{
	auto width	= mBounds.width / 2.f;
	auto height = mBounds.height / 2.f;
	auto x		= mBounds.left;
	auto y		= mBounds.top;

	mChildren[TopLeft]		= std::make_unique<QuadTree>(mLevel + 1, sf::FloatRect(x, y, width, height));
	mChildren[TopRight]		= std::make_unique<QuadTree>(mLevel + 1, sf::FloatRect(x + width, y, width, height));
	mChildren[BottomLeft]	= std::make_unique<QuadTree>(mLevel + 1, sf::FloatRect(x, y + height, width, height));
	mChildren[BottomRight]	= std::make_unique<QuadTree>(mLevel + 1, sf::FloatRect(x + width, y + height, width, height));
}

QuadTree::Quadrant QuadTree::getIndex(const sf::FloatRect& bounds) const
{
	assert(bounds.height > 0.f);
	assert(bounds.width > 0.f);

	auto verticalMidpoint	= mBounds.left + mBounds.width / 2.f;
	auto horizontalMidpoint = mBounds.top + mBounds.height / 2.f;

	// Can the object "completely" fit within this quadrant?
	bool top	= (bounds.top + bounds.height < horizontalMidpoint);
	bool bottom = (bounds.top > horizontalMidpoint);
	bool left	= (bounds.left + bounds.width < verticalMidpoint);
	bool right	= (bounds.left > verticalMidpoint);

	if (top && left)
		return TopLeft;

	if (top && right)
		return TopRight;

	if (bottom && left)
		return BottomLeft;

	if (bottom && right)
		return BottomRight;
	
	return NotFound;
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
	return(mChildren[0] != nullptr);
}