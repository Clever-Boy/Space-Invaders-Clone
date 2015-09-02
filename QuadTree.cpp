#include "QuadTree.hpp"

#include <SFML\Graphics\RectangleShape.hpp>
#include <SFML\Graphics\RenderTarget.hpp>

#include <cassert>


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
	float subWidth = mBounds.width / 2.f;
	float subHeight = mBounds.height / 2.f;
	float x = mBounds.left;
	float y = mBounds.top;

	mChildren[0] = std::move(Ptr(new QuadTree(mlevel + 1, sf::FloatRect(x + subWidth, y, subWidth, subHeight))));
	mChildren[1] = std::move(Ptr(new QuadTree(mlevel + 1, sf::FloatRect(x, y, subWidth, subHeight))));
	mChildren[2] = std::move(Ptr(new QuadTree(mlevel + 1, sf::FloatRect(x, y + subHeight, subWidth, subHeight))));
	mChildren[3] = std::move(Ptr(new QuadTree(mlevel + 1, sf::FloatRect(x + subWidth, y + subHeight, subWidth, subHeight))));
}

int QuadTree::getIndex(const sf::FloatRect &Rect)
{
	int index = -1;

	float verticalMidpoint = mBounds.left + (mBounds.width / 2);
	float horizontalMidpoint = mBounds.top + (mBounds.height / 2);

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

void QuadTree::insert(SceneNode* object)
{
	if (mChildren[0] != nullptr)
	{
		int index = getIndex(object->getBoundingRect());

		if (index != -1)
		{
			mChildren[index]->insert(object);

			return;
		}
	}

	mObjects.push_back(object);

	if (mObjects.size() > MAX_OBJECTS && mlevel < MAX_LEVELS)
	{
		if (mChildren[0] == nullptr)
		{
			split();
		}

		for (auto i = mObjects.cbegin(); i != mObjects.cend();)
		{
			int index = getIndex((*i)->getBoundingRect());
			if (index != -1)
			{
				SceneNode* temp = *i;
				i = mObjects.erase(i);
				mChildren[index]->insert(temp);
			}
			else
			{
				++i;
			}
		}
	}
}

void QuadTree::getCloseObjects(SceneNode* from, std::deque<SceneNode*>& returnObjects)
{
	int index = getIndex(from->getBoundingRect());

	if (index != -1 && mChildren[0] != nullptr)
	{
		mChildren[index]->getCloseObjects(from, returnObjects);
	}

	std::copy_if(mObjects.begin(), mObjects.end(), std::back_inserter(returnObjects),
		[&](const auto* i)
	{
		return !(i->getCategory() & from->getCategory()) && (i != from);
	});
}

#ifdef DEBUG
void QuadTree::draw(sf::RenderTarget& target)
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

	if (mChildren[0] != nullptr)
	{
		for (const auto& child : mChildren)
			if(child)
			child->draw(target);
	}
}

#endif