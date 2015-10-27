#pragma once


#include <SFML/Window/Keyboard.hpp>
#include <SFML/System/Vector2.hpp>

#include <string>


namespace sf
{
	class Sprite;
}


namespace utility
{
	// Call setOrigin() with the center of the object
	template <typename Resource>
	void centerOrigin(Resource& resource);

	// Convert enumerators to strings
	std::string	toString(sf::Keyboard::Key key);

	void setScaleSize(sf::Sprite& sprite, float x, float y);

	// Degree/radian conversion
	float toDegree(float radian);
	float toRadian(float degree);
}

#include "Utility.inl"
