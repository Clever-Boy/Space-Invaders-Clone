#pragma once


#include "SceneNode.hpp"

#include <deque>
#include <array>
#include <memory>


class QuadTree : private sf::NonCopyable
{
public:
	using Ptr = std::unique_ptr<QuadTree>;
	using ObjectsContainer = std::deque<SceneNode*>;


public:
	explicit QuadTree(std::size_t level, const sf::FloatRect& bounds);
	~QuadTree();

	void                    setBounds(const sf::FloatRect& bounds);

	void                    insert(SceneNode *object);

	void                    clear();
	const sf::FloatRect&    getBounds() const;
	void					clean();
	void                    getCloseObjects(SceneNode* from, ObjectsContainer& returnedObjects);

#ifdef DEBUG
	void    draw(sf::RenderTarget& target);
#endif


private:
	bool                    isFinal() const;
	bool                    hasChildren() const;

	void                    split();
	int                     getChildIndex(const sf::FloatRect& rect);


private:
	sf::FloatRect				mBounds;
	std::size_t                 mLevel;
	static const std::size_t	mMaxLevel = 4;
	ObjectsContainer			mObjects;
	std::array<Ptr, mMaxLevel>	mChildren;
};
