#include "QuadTree.hpp"

#include <SFML\Graphics\RenderTarget.hpp>

#include <cassert>


QuadTree::QuadTree()
	: mBounds(sf::FloatRect())
	, mLevel()
	, mChildren()
{
}

QuadTree::QuadTree(std::size_t level, const sf::FloatRect &bounds)
	: mBounds(bounds)
	, mLevel(level)
	, mChildren()
{
	if (!isFinal())
		split();
}

QuadTree::~QuadTree()
{
	clean();
}

void QuadTree::setBounds(const sf::FloatRect &bounds)
{
	mBounds = bounds;

	for (auto&& child : mChildren)
	{
		child.release();
	}

	if (!isFinal())
		split();
}

void QuadTree::insert(SceneNode* object)
{
	// If that QuadTree node has children, we give them the object
	if (mChildren.empty())
	{
		int index = getChildIndex(object->getBoundingRect());

		if (index >= 0)
		{
			mChildren[index]->insert(object);
			return;
		}
	}

	// If there's no children or children cannot carry the objet
	mObjects.push_back(object);
}

void QuadTree::getCloseObjects(SceneNode* from, ObjectsContainer& returnedObjects)
{
	if (!mObjects.empty())
	{
		returnedObjects.insert(returnedObjects.end(), mObjects.begin(), mObjects.end());
	}
	else
	{
		int index = getChildIndex(from->getBoundingRect());

		if (index >= 0)
		{
			mChildren[index]->getCloseObjects(from, returnedObjects);
		}
	}
}

void QuadTree::clear()
{
	mObjects.clear();
	for (const auto& child : mChildren)
	{
		if (child)
		{
			child->clear();
		}
	}
}

void QuadTree::clean()
{
	mObjects.clear();
	for (auto&& child : mChildren)
	{
		if (child)
		{
			child.release();
		}
	}
}

void QuadTree::split()
{
	assert(!isFinal());

	if (mChildren.empty())
		return;

	float subWidth = mBounds.width / 2.f;
	float subHeight = mBounds.height / 2.f;
	float x = mBounds.left;
	float y = mBounds.top;

	mChildren[0] = std::move(Ptr(std::make_unique<QuadTree>(mLevel + 1, sf::FloatRect(x, y, subWidth, subHeight))));
	mChildren[1] = std::move(Ptr(std::make_unique<QuadTree>(mLevel + 1, sf::FloatRect(x + subWidth, y, subWidth, subHeight))));
	mChildren[2] = std::move(Ptr(std::make_unique<QuadTree>(mLevel + 1, sf::FloatRect(x, y + subHeight, subWidth, subHeight))));
	mChildren[3] = std::move(Ptr(std::make_unique<QuadTree>(mLevel + 1, sf::FloatRect(x + subWidth, y + subHeight, subWidth, subHeight))));
}

int QuadTree::getChildIndex(const sf::FloatRect &rect)
{
	int index = -1;

	if (!mChildren.empty())
		return index;

	for (std::size_t i = 0; i < mChildren.size(); ++i)
	{
		const sf::FloatRect& bounds = mChildren[i]->getBounds();
		if (bounds.contains(sf::Vector2f(rect.left, rect.top)) && bounds.contains(sf::Vector2f(rect.left + rect.width, rect.top + rect.height)))
			return i;
	}

	return index;
}

const sf::FloatRect &QuadTree::getBounds() const
{
	return mBounds;
}

bool QuadTree::isFinal() const
{
	return mLevel >= mMaxLevel;
}
