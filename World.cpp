#include "World.hpp"
#include "Utility.hpp"
#include "SoundNode.hpp"
#include "DataTables.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

//#define DEBUG

namespace
{
	bool collision(const SceneNode& lhs, const SceneNode& rhs)
	{
		return lhs.getBoundingRect().intersects(rhs.getBoundingRect());
	}

	template<typename GameObject>
	auto PixelcollidesPair(const Shield& shield, const GameObject& object) ->bool
	{
		auto shieldBounds = static_cast<sf::Rect<std::size_t>>(shield.getBoundingRect());
		auto objectBounds = static_cast<sf::Rect<std::size_t>>(object.getBoundingRect());

		auto width = objectBounds.left + objectBounds.width * object.getScale().x;
		auto height = objectBounds.top + objectBounds.height * object.getScale().y;

		sf::Vector2u position(objectBounds.left, objectBounds.top);

		if (!objectBounds.intersects(shieldBounds))
			return false;

		for (auto x = position.x; x < width; ++x)
		{
			for (auto y = position.y; y < height; ++y)
			{
				auto relX = x - shieldBounds.left;

				auto relY = (object.getCategory() & Category::EnemyProjectile) ? y - shieldBounds.top - objectBounds.height : y - shieldBounds.top;
				
				if (shield.getPixel(relX, relY))
					return true;
			}
		}

		return false;
	}

	constexpr auto Padding = 40.f;
	constexpr auto MovementsPadding = 55.f;
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
	, mBoss(nullptr)
	, mQuadTreePrimary(1, mWorldBounds)
	, mQuadTreeSecondary(1, mWorldBounds)
	, mEnemyNodes()
	, mPlayerBulletNodes()
	, mEnemyBulletNodes()
	, mLives()
	, mDeadLine(getBattlefieldBounds().height + getBattlefieldBounds().top)
	, mScore()
	, mStaticScoreText()
	, mScoreText()
	, mLivesText()
	, mSounds(sounds)
{
	mStaticScoreText.setString("Score: ");
	mStaticScoreText.setFont(mFonts.get(Fonts::Main));
	mStaticScoreText.setPosition(5.f, 5.f);
	mStaticScoreText.setCharacterSize(30u);

	mScoreText.setString(std::to_string(mScore));
	mScoreText.setFont(mFonts.get(Fonts::Main));
	mScoreText.setPosition(100.f, 25.f);
	mScoreText.setCharacterSize(25u);
	mScoreText.setColor(sf::Color::Green);
	centerOrigin(mScoreText);

	mLivesText.setString("Lives: ");
	mLivesText.setFont(mFonts.get(Fonts::Main));
	mLivesText.setPosition(mWorldView.getSize().x - 200.f, 5.f);
	mLivesText.setCharacterSize(30u);

	loadTextures();
	buildScene();

	// Prepare the view
	mWorldView.setCenter(mSpawnPosition);
}

void World::loadTextures()
{
	mTextures.load(Textures::Boss, "Media/Textures/Boss.png");
	mTextures.load(Textures::Player, "Media/Textures/PlayerSet.png");
	mTextures.load(Textures::PlayerExplosion, "Media/Textures/PlayerExpolsion.png");
	mTextures.load(Textures::Background, "Media/Textures/Backgoundblack.jpg");
	mTextures.load(Textures::Enemies, "Media/Textures/Enemies.png");
	mTextures.load(Textures::EnemiesExplosion, "Media/Textures/EnemiesExplosion.png");
	mTextures.load(Textures::Bullet, "Media/Textures/Bullet.png");

	mImages.load(Images::Shield, "Media/Textures/Shield.png");
}

void World::buildScene()
{
	// Initialize the different layers
	for (auto i = 0u; i < LayerCount; ++i)
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
	auto leader(std::make_unique<Player>(Player::PlayerShip, mTextures));
	mPlayerShip = leader.get();
	mPlayerShip->setPosition(mSpawnPosition + sf::Vector2f(0.f, 240.f));

	mSceneLayers[Space]->attachChild(std::move(leader));

	// Add Boss
	auto boss(std::make_unique<Boss>(Boss::BossShip, mTextures));
	mBoss = boss.get();
	mBoss->setPosition(MovementsPadding, Padding * 1.5);
	mSceneLayers[Space]->attachChild(std::move(boss));

	// Add enemy Spaceships
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
	addShield(85.f, 150.f);
	addShield(-85.f, 150.f);
	addShield(250.f, 150.f);
	addShield(-250.f, 150.f);
}

void World::addShield(float relX, float relY)
{
	auto shield(std::make_unique<Shield>(mImages, mTarget.getSize()));
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
	auto life(std::make_unique<Life>(Player::PlayerShip, mTextures));
	life->setPosition(mSpawnPosition + sf::Vector2f(relX, relY));
	mLives.push_back(std::move(life));
}

void World::addEnemies()
{
	// Add enemies
	constexpr auto numberOfEnemies = 66u;
	constexpr auto enemiesPerRow = 11u;
	constexpr auto horizontalSpacing = 40.f;
	constexpr auto verticalSpacing = 35.f;

	const sf::Vector2f positionOfTopLeft(MovementsPadding, Padding * 2.5);

	for (auto i = 0u; i < numberOfEnemies; ++i)
	{
		sf::Vector2f position(horizontalSpacing * (i % enemiesPerRow), verticalSpacing * (i / enemiesPerRow));

		if (i < 22)
			addEnemy(Invaders::Enemy1, positionOfTopLeft.x + position.x, positionOfTopLeft.y + position.y);
		else if (i >= 22 && i < 44)
			addEnemy(Invaders::Enemy2, positionOfTopLeft.x + position.x, positionOfTopLeft.y + position.y);
		if (i >= 44)
			addEnemy(Invaders::Enemy3, positionOfTopLeft.x + position.x, positionOfTopLeft.y + position.y);
	}
}

void World::addEnemy(Invaders::Type type, float relX, float relY)
{
	auto enemy(std::make_unique<Invaders>(type, mTextures));
	enemy->setPosition(relX, relY);
	mSceneLayers[Space]->attachChild(std::move(enemy));
}

sf::FloatRect World::getViewBounds() const
{
	return sf::FloatRect(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());
}

sf::FloatRect World::getBattlefieldBounds() const
{
	sf::FloatRect bounds = getViewBounds();

	bounds.top += Padding;
	bounds.height -= Padding * 2;

	bounds.left += Padding;
	bounds.width -= Padding * 2;

	return bounds;
}

sf::FloatRect World::getMovementsfieldBounds() const
{
	sf::FloatRect bounds = getViewBounds();

	bounds.top += MovementsPadding;
	bounds.height -= MovementsPadding * 2 ;

	bounds.left += MovementsPadding;
	bounds.width -= MovementsPadding * 2;

	return bounds;
}

CommandQueue& World::getCommandQueue()
{
	return mCommandQueue;
}

void World::draw()
{
	mTarget.setView(mWorldView);

	mTarget.draw(mSceneGraph);

	for (const auto& i : mLives)
		mTarget.draw(*i);

	auto bounds = getBattlefieldBounds();

	sf::Vertex line[] =
	{
		sf::Vertex(sf::Vector2f(bounds.left , bounds.top + bounds.height)),
		sf::Vertex(sf::Vector2f(bounds.left + bounds.width, bounds.top + bounds.height))
	};

	line[0].color = line[1].color = sf::Color::Green;

	mTarget.draw(line, 2, sf::Lines);
	mTarget.draw(mStaticScoreText);
	mTarget.draw(mScoreText);
	mTarget.draw(mLivesText);

#ifdef DEBUG
	sf::RectangleShape shapeBattle;
	auto boundBattle = getBattlefieldBounds();
	shapeBattle.setSize(sf::Vector2f(boundBattle.width, boundBattle.height));
	shapeBattle.setFillColor(sf::Color::Transparent);
	shapeBattle.setOutlineColor(sf::Color::Red);
	shapeBattle.setOutlineThickness(2.f);
	shapeBattle.setPosition(boundBattle.left, boundBattle.top);
	mTarget.draw(shapeBattle);

	sf::RectangleShape shapeMovements;
	auto boundMovements = getMovementsfieldBounds();
	shapeMovements.setSize(sf::Vector2f(boundMovements.width, boundMovements.height));
	shapeMovements.setFillColor(sf::Color::Transparent);
	shapeMovements.setOutlineColor(sf::Color::Yellow);
	shapeMovements.setOutlineThickness(2.f);
	shapeMovements.setPosition(boundMovements.left, boundMovements.top);
	mTarget.draw(shapeMovements);

	mQuadTreePrimary.draw(mTarget);
	mQuadTreeSecondary.draw(mTarget);
#endif
}

void World::update(sf::Time dt)
{
	// reset player velocity
	mPlayerShip->setVelocity(0.f, 0.f);

	// Remove useless entities
	destroyEntitiesOutsideView();

	// Update quadtree
	checkForCollision();

	// Forward commands to scene graph
	while (!mCommandQueue.isEmpty())
		mSceneGraph.onCommand(mCommandQueue.pop());

	// Adapt Movements 
	adaptEnemyMovements();

	// control enemy fires
	controlEnemyFire();

	// Collision detection and response (may destroy entities)
	handleCollisions();

	mSceneGraph.removeWrecks();

	// Regular update step
	mSceneGraph.update(dt, mCommandQueue);

	// Adapt position (correct if outside view)
	adaptPlayerPosition();

	updateText();

	updateSounds();
}

void World::updateText()
{
	mScoreText.setString(std::to_string(mScore));
}

void World::adaptPlayerPosition()
{
	// Keep player's position inside the screen bounds, at least borderDistance units from the border
	sf::FloatRect viewBounds(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());

	sf::Vector2f position = mPlayerShip->getPosition();

	position.x = std::max(position.x, viewBounds.left + MovementsPadding);
	position.x = std::min(position.x, viewBounds.left + viewBounds.width - MovementsPadding);

	mPlayerShip->setPosition(position);
}

void World::destroyEntitiesOutsideView()
{
	Command command;
	command.category = Category::Projectile;
	command.action = derivedAction<Entity>([this](auto& entity)
	{
		if (!getBattlefieldBounds().intersects(entity.getBoundingRect()))
			entity.remove();
	});

	mCommandQueue.push(command);
}

void World::checkForCollision()
{
	mQuadTreePrimary.clear();
	mQuadTreeSecondary.clear();

	mEnemyNodes.clear();
	mPlayerBulletNodes.clear();
	mEnemyBulletNodes.clear();

	Command command;
	command.category = Category::All;
	command.action = [this](auto& node)
	{
		if (node.getCategory() & Category::PlayerProjectile)
		{
			mPlayerBulletNodes.push_back(&node);
		}
		else if (node.getCategory() & Category::EnemyProjectile)
		{
			mEnemyBulletNodes.push_back(&node);
		}
		else if (node.getCategory() & Category::EnemySpaceship)
		{
			mEnemyNodes.push_back(&node);
			mQuadTreePrimary.insert(node);
		}
		else if (node.getCategory() & Category::Shield)
		{
			mQuadTreeSecondary.insert(node);
			mQuadTreePrimary.insert(node);
		}
		else if (node.getCategory() & Category::PlayerSpaceship)
		{
			mQuadTreeSecondary.insert(node);
		}
		else if (node.getCategory() & Category::BossSpaceship)
		{
			mQuadTreePrimary.insert(node);
		}
	};

	mCommandQueue.push(command);
}

void World::handleCollisions()
{
	enemyProjectileCollision();
	playerProjectileCollision();
	enemyCollision();
}

void World::playerProjectileCollision()
{
	std::vector<SceneNode*> mCollidableNodes;

	for (const auto& node1 : mPlayerBulletNodes)
	{
		if (node1->isDestroyed())
			continue;

		mCollidableNodes.clear();
		mQuadTreePrimary.getCloseObjects(node1->getBoundingRect(), mCollidableNodes);

		for (const auto& node2 : mCollidableNodes)
		{
			if (node2->isDestroyed())
				continue;

			if (node2->getCategory() & Category::Shield)
			{
				auto& shield = static_cast<Shield&>(*node2);
				auto& projectile = static_cast<Projectile&>(*node1);

				if (PixelcollidesPair(shield, projectile))
				{
					shield.onHit(projectile.getBoundingRect(), projectile.getPosition(), projectile.getCategory());
					projectile.destroy();
				}
			}
			else if (node2->getCategory() & Category::BossSpaceship)
			{
				if (!collision(*node1, *node2))
					continue;

				auto& enemy = static_cast<Boss&>(*node2);
				auto& projectile = static_cast<Projectile&>(*node1);

				mScore += 100;

				enemy.damage(projectile.getDamage());
				projectile.destroy();
			}
			else if (node2->getCategory() & Category::EnemySpaceship)
			{
				if (!collision(*node1, *node2))
					continue;

				auto& enemy = static_cast<Invaders&>(*node2);
				auto& projectile = static_cast<Projectile&>(*node1);

				switch (enemy.getType())
				{
				case Invaders::Enemy1:
					mScore += 30;
					break;
				case Invaders::Enemy2:
					mScore += 20;
					break;
				case Invaders::Enemy3:
					mScore += 10;
					break;
				default:break;
				}

				enemy.damage(projectile.getDamage());
				projectile.destroy();
			}
		}	
	}
}

void World::enemyProjectileCollision()
{
	std::vector<SceneNode*> mCollidableNodes;

	for (const auto& node1 : mEnemyBulletNodes)
	{
		if (node1->isDestroyed())
			continue;

		mCollidableNodes.clear();
		mQuadTreeSecondary.getCloseObjects(node1->getBoundingRect(), mCollidableNodes);

		for (const auto& node2 : mCollidableNodes)
		{
			if (node2->isDestroyed())
				continue;

			if (node2->getCategory() & Category::Shield)
			{
				auto& shield = static_cast<Shield&>(*node2);
				auto& projectile = static_cast<Projectile&>(*node1);

				if (PixelcollidesPair(shield, projectile))
				{
					shield.onHit(projectile.getBoundingRect(), projectile.getPosition(), projectile.getCategory());
					projectile.destroy();
				}
			}
			else if (node2->getCategory() & Category::PlayerSpaceship)
			{
				if (!collision(*node1, *node2))
					continue;

				auto& player = static_cast<Player&>(*node2);
				auto& projectile = static_cast<Projectile&>(*node1);

				player.onHit();
				player.damage(projectile.getDamage());

				projectile.destroy();

				if (!mLives.empty())
					mLives.pop_back();
			}
		}
	}
}

void World::enemyCollision()
{
	std::vector<SceneNode*> mCollidableNodes;

	for (const auto& node1 : mEnemyNodes)
	{
		if (node1->isDestroyed())
			continue;

		mCollidableNodes.clear();
		mQuadTreeSecondary.getCloseObjects(node1->getBoundingRect(), mCollidableNodes);

		for (const auto& node2 : mCollidableNodes)
		{
			if (node2->isDestroyed())
				continue;

			if (node2->getCategory() & Category::Shield)
			{
				auto& shield = static_cast<Shield&>(*node2);
				auto& enemy = static_cast<Invaders&>(*node1);

				if (PixelcollidesPair(shield, enemy))
				{
					shield.onHit(enemy.getBoundingRect(), enemy.getPosition(), enemy.getCategory());
				}
			}
			else if(node2->getCategory() & Category::PlayerSpaceship)
			{
				if (!collision(*node1, *node2))
					continue;

				auto& player = static_cast<Player&>(*node2);
				auto& enemy = static_cast<Invaders&>(*node1);

				player.damage(enemy.getHitpoints());

				enemy.destroy();

				if (!mLives.empty())
					mLives.pop_back();
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
	return mEnemyNodes.empty() && (mBoss->isMarkedForRemoval() || mBoss == nullptr);
}

void World::adaptEnemyMovements()
{
	bool changeDirection = false;

	for (const auto& i : mEnemyNodes)
	{
		Invaders& enemy = static_cast<Invaders&>(*i);

		if (!getMovementsfieldBounds().contains(enemy.getPosition()))
			changeDirection = true;
	}

	// let invaders moving down and update condition of change direction
	for (const auto& i : mEnemyNodes)
	{
		Invaders& enemy = static_cast<Invaders&>(*i);

		enemy.requestChangeDirection(changeDirection);
	}
}

void World::controlEnemyFire()
{
	// Sort all enemies according to their y value, such that lower enemies are ready to fire
	std::sort(mEnemyNodes.begin(), mEnemyNodes.end(),
		[this](const auto& lhs, const auto& rhs)
	{
		return lhs->getPosition().y > rhs->getPosition().y;
	});

	auto size = mEnemyNodes.size();

	for (auto i = 0u; i < size; ++i)
	{
		Invaders& enemy = static_cast<Invaders&>(*mEnemyNodes[i]);

		if (enemy.getWorldPosition().y >= mDeadLine - 20.f)
			mPlayerShip->destroy();

		if (mEnemyNodes.size() <= 3)
			enemy.setMaxSpeed(enemy.getMaxSpeed() + 1.f);

		if (i < 11)
			enemy.fire();
	}
}

void World::updateSounds()
{
	// Set listener's position to player position
	mSounds.setListenerPosition(mPlayerShip->getWorldPosition());

	// Remove unused sounds
	mSounds.removeStoppedSounds();
}