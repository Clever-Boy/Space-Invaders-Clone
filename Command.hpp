#pragma once


#include "Category.hpp"

#include <functional>
#include <cassert>


class SceneNode;


struct Command
{
	using Action = std::function<void(SceneNode&)>;

					Command();
	Action			action;
	unsigned int	category;
};

template <typename GameObject, typename Function>
Command::Action derivedAction(Function fn)
{
	return [=](SceneNode& node)
	{
		// Check if cast is safe
		assert(dynamic_cast<GameObject*>(&node) != nullptr);

		// Downcast node and invoke function on it
		fn(static_cast<GameObject&>(node));
	};
}
