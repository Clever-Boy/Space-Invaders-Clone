#include <cmath>

template <typename Resource>
void utility::centerOrigin(Resource& resource)
{
	static_assert(	std::is_base_of<sf::Sprite, Resource>::value || 
					std::is_base_of<sf::Text, Resource>::value ||
					std::is_base_of<sf::CircleShape, Resource>::value, "!");

	auto bounds(resource.getLocalBounds());
	resource.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
}