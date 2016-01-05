#pragma once


#include <SFML/Window/Keyboard.hpp>

#include <string>


namespace sf
{
	class Sprite;
	class Text;
	class CircleShape;
}


namespace utility
{
	template <typename Resource>
	void centerOrigin(Resource& resource);

	void setScaleSize(sf::Sprite& sprite, float x, float y);

	float radian(std::size_t direction);

	std::string	toString(sf::Keyboard::Key key);
}


#include "Utility.inl"
