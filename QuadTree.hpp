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

	///////////////////////////////////////////////////////////////////
	// Clears the quadtree                                           //
	///////////////////////////////////////////////////////////////////
	void								clear();


	///////////////////////////////////////////////////////////////////
	// Insert the object into the quadtree. If the node              //
	// exceeds the capacity, it will split and add all               //
	// objects to their corresponding nodes.                         //
	///////////////////////////////////////////////////////////////////
	void								insert(SceneNode* object);


	///////////////////////////////////////////////////////////////////
	// Return all objects that could collide with the given object   //
	///////////////////////////////////////////////////////////////////
	void								getCloseObjects(SceneNode* from, std::vector<SceneNode*>& returnObjects);


#ifdef DEBUG
	void								draw(sf::RenderTarget& target);
#endif


private:

	///////////////////////////////////////////////////////////////////
	// Splits the node into 4 subnodes                               //
	///////////////////////////////////////////////////////////////////
	void								split();


	///////////////////////////////////////////////////////////////////
	// Determine which node the object belongs to. -1 means          //
	// object cannot completely fit within a child node and is part  //
	// of the parent node                                            //
	///////////////////////////////////////////////////////////////////
	int									getIndex(const sf::FloatRect &Rect);


private:
	static constexpr std::size_t		MAX_LEVELS		= 5; //defines the deepest level subnode
	static constexpr std::size_t		MAX_OBJECTS		= 2; //defines how many objects a node can hold before it splits
	static constexpr std::size_t		DEFAULT_NODES	= 4;

	sf::FloatRect						mBounds;
	std::size_t							mlevel;

	std::vector<SceneNode*>				mObjects;
	std::array<Ptr, DEFAULT_NODES>		mChildren;
};
