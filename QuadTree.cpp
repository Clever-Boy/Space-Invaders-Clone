#include "QuadTree.hpp"


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

QuadTree::~QuadTree()
{
	clear();
}

void QuadTree::clear()
{
	mObjects.clear();

	for (auto&& child : mChildren)
	{
		if (child != nullptr)
		{
			child->clear();
			child.reset(nullptr);
		}
	}
}

void QuadTree::split()
{
	auto width	= mBounds.width / 2.f;
	auto height = mBounds.height / 2.f;
	auto x		= mBounds.left;
	auto y		= mBounds.top;

	mChildren[0] = std::move(std::make_unique<QuadTree>(mlevel + 1, sf::FloatRect(x + width, y, width, height)));
	mChildren[1] = std::move(std::make_unique<QuadTree>(mlevel + 1, sf::FloatRect(x, y, width, height)));
	mChildren[2] = std::move(std::make_unique<QuadTree>(mlevel + 1, sf::FloatRect(x, y + height, width, height)));
	mChildren[3] = std::move(std::make_unique<QuadTree>(mlevel + 1, sf::FloatRect(x + width, y + height, width, height)));
}

int QuadTree::getIndex(const sf::FloatRect& Rect) const
{
	auto index = -1;

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
			index = 1;
		}
		else if (bottomQuadrant)
		{
			index = 2;
		}
	}

	// Object can completely fit within the right quadrants
	else if (Rect.left > verticalMidpoint)
	{
		if (topQuadrant)
		{
			index = 0;
		}
		else if (bottomQuadrant)
		{
			index = 3;
		}
	}

	return index;
}

void QuadTree::insert(SceneNode& object)
{
	if (mChildren[0] != nullptr)
	{
		auto index = getIndex(object.getBoundingRect());

		if (index != -1)
		{
			mChildren[index]->insert(object);

			return;
		}
	}

	mObjects.push_back(&object);

	if (mObjects.size() < MaxObjects && mlevel > MaxLevels)
		return;

	if (mChildren[0] == nullptr)
	{
		split();
	}

	for (auto i = mObjects.cbegin(); i != mObjects.cend();)
	{
		int index = getIndex((*i)->getBoundingRect());

		if (index != -1)
		{
			auto& temp(**i);
			i = mObjects.erase(i);
			mChildren[index]->insert(temp);
		}
		else
		{
			++i;
		}
	}
}

void QuadTree::getCloseObjects(const sf::FloatRect& Bounds, ObjectsContainer& returnObjects) const
{
	auto index = getIndex(Bounds);

	if (index != -1 && mChildren[0] != nullptr)
	{
		mChildren[index]->getCloseObjects(Bounds, returnObjects);
	}

	std::copy(mObjects.begin(), mObjects.end(), std::back_inserter(returnObjects));
}