#include "QuadTree.hpp"

#include <SFML\Graphics\RectangleShape.hpp>
#include <SFML\Graphics\RenderTarget.hpp>

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
	, mLevel(Level)
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

void QuadTree::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::RectangleShape shape(sf::Vector2f(mBounds.width, mBounds.height));

	shape.setPosition(mBounds.left, mBounds.top);

	if (mObjects.empty())
		shape.setFillColor(sf::Color(0, 0, 0, 0));
	else
		shape.setFillColor(sf::Color(255, 125, 125, 100));

	shape.setOutlineThickness(1);
	shape.setOutlineColor(sf::Color(255, 255, 255));

	target.draw(shape);

	if (!hasChildren())
		return;

	for (const auto& child : mChildren)
		child->draw(target, states);
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

QuadTree::Quadrant QuadTree::getIndex(const sf::FloatRect& Rect) const
{
	assert(Rect.height > 0.f);
	assert(Rect.width > 0.f);

	auto verticalMidpoint	= mBounds.left + mBounds.width / 2.f;
	auto horizontalMidpoint = mBounds.top + mBounds.height / 2.f;

	// Can the object "completely" fit within this quadrant?
	bool top	= (Rect.top + Rect.height < horizontalMidpoint);
	bool bottom = (Rect.top > horizontalMidpoint);
	bool left	= (Rect.left + Rect.width < verticalMidpoint);
	bool right	= (Rect.left > verticalMidpoint);

	if (top && left)
		return TopLeft;

	if (bottom && left)
		return BottomLeft;

	if (top && right)
		return TopRight;

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