#pragma once


#include "ResourceIdentifiers.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>

#include <vector>


struct Direction
{
	Direction(float angle, float distance)
		: angle(angle)
		, distance(distance)
	{
	}

	float angle;
	float distance;
};

struct SpaceshipData
{
	int								hitpoints;
	float							speed;
	Textures::ID					texture;
	sf::IntRect						textureRect;
	sf::Color						color;
	sf::Vector2f					size;
	sf::Time						fireInterval;
	std::size_t						fireRate;
	std::vector<Direction>			directions;
	bool							hasAnimation;
	sf::Time						animationInterval;
	int animateRate;
	sf::IntRect						textureRectExplosion;
};

struct ProjectileData
{
	int								damage;
	float							speed;
	sf::Color						color;
	sf::Vector2f					size;
	Textures::ID					texture;
	sf::IntRect						textureRect;
};

std::vector<SpaceshipData>	initializeSpaceshipData();
std::vector<ProjectileData>	initializeProjectileData();