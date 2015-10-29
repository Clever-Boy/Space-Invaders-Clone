#include "SceneNode.hpp"
#include "Command.hpp"


SceneNode::SceneNode(Category::Type category)
	: mChildren()
	, mParent(nullptr)
	, mDrity(true)
	, mDefaultCategory(category)
{
}

void SceneNode::attachChild(Ptr child)
{
	child->mParent = this;
	mChildren.push_back(std::move(child));
}

void SceneNode::update(sf::Time dt, CommandQueue& commands)
{
	updateCurrent(dt, commands);
	updateChildren(dt, commands);
}

sf::Vector2f SceneNode::getWorldPosition() const
{
	return getWorldTransform() * sf::Vector2f();
}

void SceneNode::onCommand(const Command& command)
{
	if (command.category & getCategory())
		command.action(*this);

	for (const auto& child : mChildren)
		child->onCommand(command);
}

unsigned int SceneNode::getCategory() const
{
	return mDefaultCategory;
}

void SceneNode::removeWrecks()
{
	mChildren.erase(
		std::remove_if(mChildren.begin(), mChildren.end(), 
			std::mem_fn(&SceneNode::isMarkedForRemoval)), 
		mChildren.end());

	std::for_each(mChildren.begin(), mChildren.end(), std::mem_fn(&SceneNode::removeWrecks));
}

sf::FloatRect SceneNode::getBoundingRect() const
{
	return sf::FloatRect();
}

bool SceneNode::isDestroyed() const
{
	return false;
}

void SceneNode::setDirtyFlag(bool flag)
{
	mDrity = flag;
}

sf::Transform SceneNode::getWorldTransform() const
{
	auto transform(sf::Transform::Identity);

	for (const auto* node = this; node != nullptr; node = node->mParent)
		transform.combine(node->getTransform());

	return transform;
}

void SceneNode::updateCurrent(sf::Time dt, CommandQueue& commands)
{
}

void SceneNode::updateChildren(sf::Time dt, CommandQueue& commands)
{
	for (const auto& child : mChildren)
		child->update(dt, commands);
}

void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (mDrity)
		states.transform.combine(getTransform());

	drawCurrent(target, states);
	drawChildren(target, states);
}

void SceneNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
}

void SceneNode::drawChildren(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& child : mChildren)
		child->draw(target, states);
}

bool SceneNode::isMarkedForRemoval() const
{
	return isDestroyed();
}