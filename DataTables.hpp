#pragma once


#include "ResourceIdentifiers.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>

#include <vector>

struct BossData
{
	int								hitpoints;
	float							speed;
	Textures::ID					texture;
	sf::Color						color;
	sf::Vector2f					size;
};

struct InvadersData
{
	int								hitpoints;
	float							speed;
	Textures::ID					texture;
	sf::IntRect						textureRect;
	sf::Color						color;
	sf::Vector2f					size;
	sf::Time						fireInterval;
	std::size_t						fireRate;
	sf::Time						animationInterval;
	int								animateRate;
};

struct PlayerData
{
	int								hitpoints;
	float							speed;
	Textures::ID					texture;
	sf::IntRect						textureRect;
	sf::Vector2f					size;
	sf::Time						animationInterval;
	int								animateRate;
};

struct ProjectileData
{
	int								hitpoints;
	int								damage;
	float							speed;
	sf::Vector2f					size;
	Textures::ID					texture;
};

std::vector<BossData>		initializeBossData();
std::vector<InvadersData>	initializeInvadersData();
std::vector<PlayerData>		initializePlayerData();
std::vector<ProjectileData>	initializeProjectileData();
