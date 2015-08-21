#include "World.hpp"
#include "Utility.hpp"
#include "SoundNode.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <algorithm>
#include <cmath>


namespace
{
	bool matchesCategories(SceneNode::Pair& colliders, Category::Type type1, Category::Type type2)
	{
		unsigned int category1 = colliders.first->getCategory();
		unsigned int category2 = colliders.second->getCategory();

		// Make sure first pair entry has category type1 and second has type2
		if (type1 & category1 && type2 & category2)
		{
			return true;
		}
		else if (type1 & category2 && type2 & category1)
		{
			std::swap(colliders.first, colliders.second);
			return true;
		}
		else
		{
			return false;
		}
	}

	bool collision(const SceneNode& lhs, const SceneNode& rhs)
	{
		return lhs.getBoundingRect().intersects(rhs.getBoundingRect());
	}
}

World::World(sf::RenderTarget& outputTarget, FontHolder& fonts, SoundPlayer& sounds)
	: mTarget(outputTarget)
	, mWorldView(outputTarget.getDefaultView())
	, mTextures()
	, mFonts(fonts)
	, mSceneGraph()
	, mSceneLayers()
	, mWorldBounds(0.f, 0.f, mWorldView.getSize().x, mWorldView.getSize().y)
	, mSpawnPosition(mWorldView.getSize().x / 2.f, mWorldView.getSize().y / 2.f)
	, mPlayerShip(nullptr)
	, mQuadTree()
	, mCollidableNodes()
	, mActiveEnemies()
	, mLives()
	, mDeadLine(getBattlefieldBounds().height + getBattlefieldBounds().top)
	, mScore()
	, mStaticScoreText()
	, mScoreText()
	, mLivesText()
	, mSounds(sounds)
	//, mGameOver(false)
{
	mStaticScoreText.setString("Score: ");
	mStaticScoreText.setFont(mFonts.get(Fonts::Main));
	mStaticScoreText.setPosition(5.f, 5.f);
	mStaticScoreText.setCharacterSize(30);

	mScoreText.setString(std::to_string(mScore));
	mScoreText.setFont(mFonts.get(Fonts::Main));
	mScoreText.setPosition(100.f, 25.f);
	mScoreText.setCharacterSize(25);
	mScoreText.setColor(sf::Color::Green);
	centerOrigin(mScoreText);

	mLivesText.setString("Lives: ");
	mLivesText.setFont(mFonts.get(Fonts::Main));
	mLivesText.setPosition(mWorldView.getSize().x - 200.f, 5.f);
	mLivesText.setCharacterSize(30);

	loadTextures();
	buildScene();

	// Prepare the view
	mWorldView.setCenter(mSpawnPosition);
}

sf::FloatRect World::getViewBounds() const
{
	return sf::FloatRect(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());
}

sf::FloatRect World::getBattlefieldBounds() const
{
	sf::FloatRect bounds = getViewBounds();
	bounds.top += 40.f;
	bounds.height -= 40.f * 2;

	bounds.left += 40.f;
	bounds.width -= 40.f * 2;

	return bounds;
}

void World::update(sf::Time dt)
{
	// reset player velocity
	mPlayerShip->setVelocity(0.f, 0.f);

	// control enemy fires
	controlEnemyFire();

	// Remove useless entities
	destroyEntitiesOutsideView();

	// Update quadtree
	checkForCollision();

	// Forward commands to scene graph
	while (!mCommandQueue.isEmpty())
		mSceneGraph.onCommand(mCommandQueue.pop());

	// Collision detection and response (may destroy entities)
	handleCollisions();

	mSceneGraph.removeWrecks();

	// Regular update step, adapt position (correct if outside view)
	mSceneGraph.update(dt, mCommandQueue);

	adaptPlayerPosition();

	updateText();

	updateSounds();
}

void World::updateText()
{
	mScoreText.setString(std::to_string(mScore));
}

void World::draw()
{
	mTarget.setView(mWorldView);
	mTarget.draw(mSceneGraph);

	for (const auto& i : mLives)
		mTarget.draw(*i);


	sf::Vertex line[] =
	{
		sf::Vertex(sf::Vector2f(getBattlefieldBounds().left , getBattlefieldBounds().top + getBattlefieldBounds().height)),
		sf::Vertex(sf::Vector2f(getBattlefieldBounds().left + getBattlefieldBounds().width, getBattlefieldBounds().top + getBattlefieldBounds().height))
	};

	line[0].color = sf::Color::Green;
	line[1].color = sf::Color::Green;

	mTarget.draw(line, 2, sf::Lines);
	mTarget.draw(mStaticScoreText);
	mTarget.draw(mScoreText);
	mTarget.draw(mLivesText);
}

CommandQueue& World::getCommandQueue()
{
	return mCommandQueue;
}

void World::loadTextures()
{
	mTextures.load(Textures::Boss, "Media/Textures/Boss.png");
	mTextures.load(Textures::Player, "Media/Textures/PlayerSet.png");
	mTextures.load(Textures::PlayerExplosion, "Media/Textures/PlayerExpolsion.png");
	mTextures.load(Textures::Shields, "Media/Textures/Shields.png");
	mTextures.load(Textures::Background, "Media/Textures/Backgoundblack.jpg");
	mTextures.load(Textures::Enemies, "Media/Textures/Enemies.png");
	mTextures.load(Textures::EnemiesExplosion, "Media/Textures/EnemiesExplosion.png");
	mTextures.load(Textures::Bullet, "Media/Textures/Bullet.png");
}

void World::buildScene()
{
	// Initialize the different layers
	for (std::size_t i = 0; i < LayerCount; ++i)
	{
		Category::Type category = (i == Space) ? Category::SceneSpaceLayer : Category::None;

		auto layer(std::make_unique<SceneNode>(category));
		mSceneLayers[i] = layer.get();

		mSceneGraph.attachChild(std::move(layer));
	}

	//Prepare the tiled background
	sf::Texture& texture = mTextures.get(Textures::Background);

	// Add the background sprite to the scene
	auto backgroundSprite(std::make_unique<SpriteNode>(texture));
	backgroundSprite->setPosition(mWorldBounds.left, mWorldBounds.top);
	mSceneLayers[Background]->attachChild(std::move(backgroundSprite));

	// Add player's spaceship
	auto leader(std::make_unique<Spaceship>(Spaceship::Player, mTextures));
	mPlayerShip = leader.get();
	mPlayerShip->setPosition(mSpawnPosition + sf::Vector2f(0.f, 240.f));

	mSceneLayers[Space]->attachChild(std::move(leader));

	// Add enemy aircraft
	addEnemies();

	// add lifes dummys
	addLifes();

	// add Shields
	addShields();

	// Add sound effect node
	auto soundNode(std::make_unique<SoundNode>(mSounds));
	mSceneGraph.attachChild(std::move(soundNode));
}

void World::addShields()
{
	sf::Vector2f position = sf::Vector2f(85.f, 150.f);
	adaptShieldPlaces(position);

	position = sf::Vector2f(-85.f, 150.f);
	adaptShieldPlaces(position);

	position = sf::Vector2f(250.f, 150.f);
	adaptShieldPlaces(position);

	position = sf::Vector2f(-250.f, 150.f);
	adaptShieldPlaces(position);
}

void World::adaptShieldPlaces(sf::Vector2f position)
{
	sf::Vector2f shieldSize(15.f, 15.f);

	addShield(Shield::Middle,		position.x + shieldSize.x * 0, position.y + shieldSize.y * 0);
	addShield(Shield::TopLeft,		position.x - shieldSize.x * 1, position.y + shieldSize.y * 0);
	addShield(Shield::TopRight,		position.x + shieldSize.x * 1, position.y + shieldSize.y * 0);
	addShield(Shield::BottomLeft,	position.x - shieldSize.x * 1, position.y + shieldSize.y * 1);
	addShield(Shield::BottomRight,	position.x + shieldSize.x * 1, position.y + shieldSize.y * 1);
}

void World::addShield(Shield::Type type, float relX, float relY)
{
	auto shield(std::make_unique<Shield>(type, mTextures));
	shield->setPosition(mSpawnPosition.x + relX, mSpawnPosition.y + relY);
	mSceneLayers[Space]->attachChild(std::move(shield));
}

void World::addLifes()
{
	addLife(300.f, -275.f);
	addLife(350.f, -275.f);
}

void World::addLife(float relX, float relY)
{
	auto life(std::make_unique<Life>(Spaceship::Player, mTextures));
	life->setPosition(mSpawnPosition + sf::Vector2f(relX, relY));

	mLives.push_back(std::move(life));
}

void World::addEnemies()
{
	// Add Boss
	addEnemy(Spaceship::Boss, mWorldBounds.width / 2 - 40.f, 210.f);

	// Add enemies
	sf::Vector2f position = sf::Vector2f(40.f, 190.f);
	std::size_t colHight = 35;

	for (std::size_t i = 0; i < 6; ++i)
	{
		for (std::size_t j = 0; j < 6; ++j)
		{
			if (j >= 0 && j < 2)
			{
				if (i != 0)
				{
					addEnemy(Spaceship::Enemy1, +position.x * i, position.y - colHight * j);
					addEnemy(Spaceship::Enemy1, -position.x * i, position.y - colHight * j);
				}
				else
					addEnemy(Spaceship::Enemy1, position.x * i, position.y - colHight * j);
			}

			if (j >= 2 && j < 4)
			{
				if (i != 0)
				{
					addEnemy(Spaceship::Enemy2, +position.x * i, position.y - colHight * j);
					addEnemy(Spaceship::Enemy2, -position.x * i, position.y - colHight * j);
				}
				else
					addEnemy(Spaceship::Enemy2, position.x * i, position.y - colHight * j);
			}

			if (j >= 4 && j < 6)
			{
				if (i != 0)
				{
					addEnemy(Spaceship::Enemy3, +position.x * i, position.y - colHight * j);
					addEnemy(Spaceship::Enemy3, -position.x * i, position.y - colHight * j);
				}
				else
					addEnemy(Spaceship::Enemy3, position.x * i, position.y - colHight * j);
			}
		}
	}
}

void World::addEnemy(Spaceship::Type type, float relX, float relY)
{
	auto enemy(std::make_unique<Spaceship>(type, mTextures));
	enemy->setPosition(mSpawnPosition.x + relX, mSpawnPosition.y - relY);
	mSceneLayers[Space]->attachChild(std::move(enemy));
}

void World::adaptPlayerPosition()
{
	// Keep player's position inside the screen bounds, at least borderDistance units from the border
	sf::FloatRect viewBounds(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());
	const float borderDistance = 40.f;

	sf::Vector2f position = mPlayerShip->getPosition();
	position.x = std::max(position.x, viewBounds.left + borderDistance);
	position.x = std::min(position.x, viewBounds.left + viewBounds.width - borderDistance);
	mPlayerShip->setPosition(position);
}

void World::destroyEntitiesOutsideView()
{
	Command command;
	command.category = Category::Projectile;
	command.action = derivedAction<Entity>([this](Entity& entity)
	{
		if (!getBattlefieldBounds().intersects(entity.getBoundingRect()))
			entity.remove();
	});

	mCommandQueue.push(command);
}

void World::checkForCollision()
{
	mQuadTree.setBounds(getViewBounds());

	mQuadTree.clear();
	mCollidableNodes.clear();

	Command command;
	command.category = Category::All;
	command.action = [this](SceneNode& node)
	{
		mQuadTree.insert(&node);
		mCollidableNodes.push_back(&node);
	};

	mCommandQueue.push(command);
}

void World::handleCollisions()
{
	std::deque<SceneNode*> proxim;
	std::set<SceneNode::Pair> checked;

	for (SceneNode* node1 : mCollidableNodes)
	{
		proxim.clear();
		mQuadTree.getCloseObjects(node1, proxim);

		// Check proxim collisions here
		for (SceneNode* node2 : proxim)
		{
			if (node1 == node2)
				continue;

			if (node1->isDestroyed() || node2->isDestroyed())
				continue;

			if (!collision(*node1, *node2))
				continue;

			SceneNode::Pair pair(std::minmax(node1, node2));

			if (checked.find(pair) != checked.end())
				continue;

			checked.insert(pair);

			if (matchesCategories(pair, Category::PlayerSpaceship, Category::EnemySpaceship))
			{
				auto& player = static_cast<Spaceship&>(*pair.first);
				auto& enemy = static_cast<Spaceship&>(*pair.second);

				player.damage(enemy.getHitpoints());
				enemy.destroy();
				if (!mLives.empty())
					mLives.pop_back();
			}
			else if (matchesCategories(pair, Category::Shield, Category::EnemySpaceship))
			{
				auto& shield = static_cast<Shield&>(*pair.first);
				auto& enemy = static_cast<Spaceship&>(*pair.second);

				shield.damage(enemy.getHitpoints());
				//enemy.destroy();
			}
			else if (matchesCategories(pair, Category::Shield, Category::PlayerProjectile)
				|| matchesCategories(pair, Category::Shield, Category::EnemyProjectile))
			{
				auto& shield = static_cast<Shield&>(*pair.first);
				auto& projectile = static_cast<Projectile&>(*pair.second);

				shield.damage(projectile.getDamage());
				projectile.destroy();
			}
			else if (matchesCategories(pair, Category::PlayerSpaceship, Category::EnemyProjectile))
			{
				auto& player = static_cast<Spaceship&>(*pair.first);
				auto& projectile = static_cast<Projectile&>(*pair.second);

				player.onHit();
				player.damage(projectile.getDamage());
				projectile.destroy();
				if (!mLives.empty())
					mLives.pop_back();
			}
			else if (matchesCategories(pair, Category::EnemySpaceship, Category::PlayerProjectile))
			{
				auto& enemy = static_cast<Spaceship&>(*pair.first);
				auto& projectile = static_cast<Projectile&>(*pair.second);

				switch (enemy.getType())
				{
				case Spaceship::Boss:
					mScore += 100;
					break;
				case Spaceship::Enemy1:
					mScore += 30;
					break;
				case Spaceship::Enemy2:
					mScore += 20;
					break;
				case Spaceship::Enemy3:
					mScore += 10;
					break;
				default:
					break;
				}
				enemy.damage(projectile.getDamage());
				projectile.destroy();
			}
		}
	}
}

bool World::hasAlivePlayer() const
{
	return !mPlayerShip->isMarkedForRemoval();
}

bool World::hasPlayerWon() const
{
	return mActiveEnemies.size() == 0;
}

void World::controlEnemyFire()
{
	// Setup command that stores all enemies in mActiveEnemies
	Command enemyCollector;
	enemyCollector.category = Category::EnemySpaceship;
	enemyCollector.action = derivedAction<Spaceship>([this](Spaceship& enemy)
	{
		if (!enemy.isDestroyed())
			mActiveEnemies.push_back(&enemy);
	});

	// Sort all enemies according to their y value, such that lower enemies are ready to fire
	std::sort(mActiveEnemies.begin(), mActiveEnemies.end(), [this](const Spaceship* lhs, const Spaceship* rhs)
	{
		return lhs->getPosition().y > rhs->getPosition().y;
	});


	std::size_t j = 0;

	// control fire and speed notify when alain reach the end screen 
	std::for_each(mActiveEnemies.begin(), mActiveEnemies.end(),
		[this, &j](Spaceship* i)
	{

		if (i->getWorldPosition().y >= mDeadLine - 20.f)
		{
			mPlayerShip->destroy();// = true;
		}


		if (mActiveEnemies.size() <= 3)
			i->setMaxSpeed(i->getMaxSpeed() + 1.f);

		if (j < 11)
			i->fire();

		++j;
	});

	// Push commands, reset active enemies
	mCommandQueue.push(enemyCollector);
	mActiveEnemies.clear();
}

void World::updateSounds()
{
	// Set listener's position to player position
	mSounds.setListenerPosition(mPlayerShip->getWorldPosition());

	// Remove unused sounds
	mSounds.removeStoppedSounds();
}