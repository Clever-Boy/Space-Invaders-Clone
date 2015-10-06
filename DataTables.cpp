#include "DataTables.hpp"
#include "Boss.hpp"
#include "Invaders.hpp"
#include "Player.hpp"
#include "Projectile.hpp"


std::vector<BossData> data::initializeBossData()
{
	std::vector<BossData> data(Boss::TypeCount);

	data[Boss::BossShip].hitpoints	= 1;
	data[Boss::BossShip].color		= sf::Color(255, 0, 0);
	data[Boss::BossShip].size		= sf::Vector2f(3.f, 1.f);
	data[Boss::BossShip].speed		= 80.f;
	data[Boss::BossShip].texture	= Textures::Boss;

	return data;
}

std::vector<InvadersData> data::initializeInvadersData()
{
	std::vector<InvadersData> data(Invaders::TypeCount);

	data[Invaders::Enemy1].hitpoints			= 1;
	data[Invaders::Enemy1].color				= sf::Color(255, 255, 255);
	data[Invaders::Enemy1].fireRate				= 1u;
	data[Invaders::Enemy1].size					= sf::Vector2f(1.5f, 1.5f);
	data[Invaders::Enemy1].speed				= 50.f;
	data[Invaders::Enemy1].texture				= Textures::Enemies;
	data[Invaders::Enemy1].textureRect			= sf::IntRect(0, 0, 25, 20);
	data[Invaders::Enemy1].fireInterval			= sf::seconds(6.f);
	data[Invaders::Enemy1].animationInterval	= sf::seconds(2.f);
	data[Invaders::Enemy1].animateRate			= 1u;
	data[Invaders::Enemy1].movement				= sf::Vector2f(1.f, 0.f);

	data[Invaders::Enemy2].hitpoints			= 1;
	data[Invaders::Enemy2].color				= sf::Color(255, 255, 255);
	data[Invaders::Enemy2].size					= sf::Vector2f(1.5f, 1.5f);
	data[Invaders::Enemy2].fireRate				= 1u;
	data[Invaders::Enemy2].speed				= 50.f;
	data[Invaders::Enemy2].texture				= Textures::Enemies;
	data[Invaders::Enemy2].textureRect			= sf::IntRect(0, 20, 25, 20);
	data[Invaders::Enemy2].fireInterval			= sf::seconds(6.f);
	data[Invaders::Enemy2].animationInterval	= sf::seconds(2.f);
	data[Invaders::Enemy2].animateRate			= 1u;
	data[Invaders::Enemy2].movement				= sf::Vector2f(1.f, 0.f);

	data[Invaders::Enemy3].hitpoints			= 1;
	data[Invaders::Enemy3].color				= sf::Color(255, 255, 255);
	data[Invaders::Enemy3].size					= sf::Vector2f(1.5f, 1.5f);
	data[Invaders::Enemy3].fireRate				= 1u;
	data[Invaders::Enemy3].speed				= 50.f;
	data[Invaders::Enemy3].texture				= Textures::Enemies;
	data[Invaders::Enemy3].textureRect			= sf::IntRect(0, 40, 25, 20);
	data[Invaders::Enemy3].fireInterval			= sf::seconds(6.f);
	data[Invaders::Enemy3].animationInterval	= sf::seconds(2.f);
	data[Invaders::Enemy3].animateRate			= 1u;
	data[Invaders::Enemy3].movement				= sf::Vector2f(1.f, 0.f);

	return data;
}

std::vector<PlayerData> data::initializePlayerData()
{
	std::vector<PlayerData> data(Player::TypeCount);

	data[Player::PlayerShip].hitpoints			= 1;
	data[Player::PlayerShip].size				= sf::Vector2f(2.f, 2.f);
	data[Player::PlayerShip].speed				= 200.f;

	data[Player::PlayerShip].texture			= Textures::Player;
	data[Player::PlayerShip].textureRect		= sf::IntRect(0, 0, 30, 20);

	data[Player::PlayerShip].animationInterval	= sf::seconds(0.25f);
	data[Player::PlayerShip].animateRate		= 4u;

	return data;
}

std::vector<ProjectileData> data::initializeProjectileData()
{
	std::vector<ProjectileData> data(Projectile::TypeCount);

	data[Projectile::PlayerBullet].hitpoints	= 1;
	data[Projectile::PlayerBullet].damage		= 1;
	data[Projectile::PlayerBullet].speed		= 600.f;
	data[Projectile::PlayerBullet].size			= sf::Vector2f(0.4f, 0.5f);
	data[Projectile::PlayerBullet].texture		= Textures::Bullet;

	data[Projectile::EnemyBullet].hitpoints		= 1;
	data[Projectile::EnemyBullet].damage		= 1;
	data[Projectile::EnemyBullet].speed			= 150.f;
	data[Projectile::EnemyBullet].size			= sf::Vector2f(0.3f, 0.6f);
	data[Projectile::EnemyBullet].texture		= Textures::Bullet;

	return data;
}
