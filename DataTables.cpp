#include "DataTables.hpp"
#include "Spaceship.hpp"
#include "Projectile.hpp"


std::vector<SpaceshipData> initializeSpaceshipData()
{
	std::vector<SpaceshipData> data(Spaceship::TypeCount);

	data[Spaceship::Player].hitpoints = 3;
	data[Spaceship::Player].color = sf::Color(0, 255, 0);
	data[Spaceship::Player].fireRate = 3;
	data[Spaceship::Player].size = sf::Vector2f(2.f, 2.f);
	data[Spaceship::Player].speed = 200.f;
	data[Spaceship::Player].fireInterval = sf::seconds(1);
	data[Spaceship::Player].texture = Textures::Player;
	data[Spaceship::Player].textureRect = sf::IntRect(0, 0, 30, 20);
	data[Spaceship::Player].hasAnimation = false;
	data[Spaceship::Player].animationInterval = sf::seconds(2);
	data[Spaceship::Player].textureRectExplosion = sf::IntRect(30, 0, 30, 20);
	data[Spaceship::Player].animateRate = 4;

	data[Spaceship::Boss].hitpoints = 1;
	data[Spaceship::Boss].color = sf::Color(255, 0, 0);
	data[Spaceship::Boss].fireRate = 0;
	data[Spaceship::Boss].size = sf::Vector2f(3.f, 1.f);
	data[Spaceship::Boss].speed = 80.f;
	data[Spaceship::Boss].fireInterval = sf::Time::Zero;
	data[Spaceship::Boss].texture = Textures::Boss;
	data[Spaceship::Boss].textureRect = sf::IntRect(0, 0, 45, 22);
	data[Spaceship::Boss].directions.push_back(Direction(-90.f, 720.f));
	data[Spaceship::Boss].directions.push_back(Direction(90.f, 720.f));
	data[Spaceship::Boss].hasAnimation = false;
	data[Spaceship::Boss].animationInterval = sf::Time::Zero;
	data[Spaceship::Boss].textureRectExplosion = sf::IntRect(0, 0, 41, 34);
	data[Spaceship::Boss].animateRate = 0;

	data[Spaceship::Enemy1].hitpoints = 1;
	data[Spaceship::Enemy1].color = sf::Color(255, 255, 255);
	data[Spaceship::Enemy1].fireRate = 1;
	data[Spaceship::Enemy1].size = sf::Vector2f(1.5f, 1.5f);
	data[Spaceship::Enemy1].speed = 50.f;
	data[Spaceship::Enemy1].texture = Textures::Enemies;
	data[Spaceship::Enemy1].textureRect = sf::IntRect(0, 0, 25, 20);
	data[Spaceship::Enemy1].directions.push_back(Direction(-90.f, 0.f));
	data[Spaceship::Enemy1].directions.push_back(Direction(0.f, 30.f));
	data[Spaceship::Enemy1].directions.push_back(Direction(+90.f, 0.f));
	data[Spaceship::Enemy1].directions.push_back(Direction(0.f, 30.f));
	data[Spaceship::Enemy1].fireInterval = sf::seconds(6);
	data[Spaceship::Enemy1].hasAnimation = true;
	data[Spaceship::Enemy1].animationInterval = sf::seconds(2);
	data[Spaceship::Enemy1].textureRectExplosion = sf::IntRect(0, 0, 41, 34);
	data[Spaceship::Enemy1].animateRate = 1;

	data[Spaceship::Enemy2].hitpoints = 1;
	data[Spaceship::Enemy2].color = sf::Color(255, 255, 255);
	data[Spaceship::Enemy2].size = sf::Vector2f(1.5f, 1.5f);
	data[Spaceship::Enemy2].fireRate = 1;
	data[Spaceship::Enemy2].speed = 50.f;
	data[Spaceship::Enemy2].texture = Textures::Enemies;
	data[Spaceship::Enemy2].textureRect = sf::IntRect(0, 20, 25, 20);
	data[Spaceship::Enemy2].directions.push_back(Direction(-90.f, 0.f));
	data[Spaceship::Enemy2].directions.push_back(Direction(0.f, 30.f));
	data[Spaceship::Enemy2].directions.push_back(Direction(+90.f, 0.f));
	data[Spaceship::Enemy2].directions.push_back(Direction(0.f, 30.f));
	data[Spaceship::Enemy2].fireInterval = sf::seconds(6);
	data[Spaceship::Enemy2].hasAnimation = true;
	data[Spaceship::Enemy2].animationInterval = sf::seconds(2);
	data[Spaceship::Enemy2].textureRectExplosion = sf::IntRect(0, 0, 41, 34);
	data[Spaceship::Enemy2].animateRate = 1;

	data[Spaceship::Enemy3].hitpoints = 1;
	data[Spaceship::Enemy3].color = sf::Color(255, 255, 255);
	data[Spaceship::Enemy3].size = sf::Vector2f(1.5f, 1.5f);
	data[Spaceship::Enemy3].fireRate = 1;
	data[Spaceship::Enemy3].speed = 50.f;
	data[Spaceship::Enemy3].texture = Textures::Enemies;
	data[Spaceship::Enemy3].textureRect = sf::IntRect(0, 40, 25, 20);
	data[Spaceship::Enemy3].directions.push_back(Direction(-90.f, 0.f ));
	data[Spaceship::Enemy3].directions.push_back(Direction(0.f, 30.f));
	data[Spaceship::Enemy3].directions.push_back(Direction(+90.f, 0.f));
	data[Spaceship::Enemy3].directions.push_back(Direction(0.f, 30.f));
	data[Spaceship::Enemy3].fireInterval = sf::seconds(6);
	data[Spaceship::Enemy3].hasAnimation = true;
	data[Spaceship::Enemy3].animationInterval = sf::seconds(2);
	data[Spaceship::Enemy3].textureRectExplosion = sf::IntRect(0, 0, 41, 34);
	data[Spaceship::Enemy3].animateRate = 1;

	return data;
}

std::vector<ProjectileData> initializeProjectileData()
{
	std::vector<ProjectileData> data(Projectile::TypeCount);

	data[Projectile::PlayerBullet].damage = 1;
	data[Projectile::PlayerBullet].speed = 300.f;
	data[Projectile::PlayerBullet].size = sf::Vector2f(0.2f, 0.5f);
	data[Projectile::PlayerBullet].texture = Textures::Bullet;
	data[Projectile::PlayerBullet].textureRect = sf::IntRect(0, 0, 1, 1);

	data[Projectile::EnemyBullet].damage = 1;
	data[Projectile::EnemyBullet].speed = 150.f;
	data[Projectile::EnemyBullet].size = sf::Vector2f(0.2f, 0.5f);
	data[Projectile::EnemyBullet].texture = Textures::Bullet;
	data[Projectile::EnemyBullet].textureRect = sf::IntRect(0, 0, 1, 1);

	return data;
}
