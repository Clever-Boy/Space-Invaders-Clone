#pragma once


#include "Category.hpp"

#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Time.hpp>

#include <utility>
#include <vector>
#include <memory>
#include <set>


struct Command;
class CommandQueue;


class SceneNode : public sf::Transformable, public sf::Drawable, private sf::NonCopyable
{
	using Ptr = std::unique_ptr<SceneNode>;


public:
	explicit				SceneNode(Category::Type category = Category::None);

	void					attachChild(Ptr child);
	Ptr						detachChild(const SceneNode& node);

	void					update(sf::Time dt, CommandQueue& commands);

	sf::Vector2f			getWorldPosition() const;
	sf::Transform			getWorldTransform() const;

	void					onCommand(const Command& command);
	virtual unsigned int	getCategory() const;


	void					removeWrecks();
	virtual sf::FloatRect	getBoundingRect() const;
	virtual bool			isDestroyed() const;
	virtual bool			isMarkedForRemoval() const;
	void					setDirtyFlag(bool flag);


private:
	virtual void			updateCurrent(sf::Time dt, CommandQueue& commands);
	void					updateChildren(sf::Time dt, CommandQueue& commands);

	void					draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	virtual void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	void					drawChildren(sf::RenderTarget& target, sf::RenderStates states) const;
	void					drawBoundingRect(sf::RenderTarget& target, sf::RenderStates states) const;


private:
	std::vector<Ptr>		mChildren;
	SceneNode*				mParent;
	mutable bool			mDrity;
	mutable sf::Transform	mTransform;
	Category::Type			mDefaultCategory;
};
