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

template <typename GameObject, class Function, typename = std::enable_if_t<std::is_base_of<SceneNode, GameObject>::value>>
auto derivedAction(Function fn) 
{
	return [=](SceneNode& node)
	{
		fn(static_cast<GameObject&>(node));
	};
}
