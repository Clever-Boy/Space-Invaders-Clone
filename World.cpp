#include "World.hpp"
#include "Utility.hpp"
#include "SoundNode.hpp"

#include <SFML/Graphics/RenderWindow.hpp>


namespace
{
	bool collision(const SceneNode& lhs, const SceneNode& rhs)
	{
		return lhs.getBoundingRect().intersects(rhs.getBoundingRect());
	}

	template<typename GameObject1, typename GameObject2>
	auto PixelcollidesPair(const GameObject1& object1, const GameObject2& object2) ->bool
	{
		auto object1Bounds = static_cast<sf::Rect<std::size_t>>(object1.getBoundingRect());
		auto object2Bounds = static_cast<sf::Rect<std::size_t>>(object2.getBoundingRect());

		auto width = object2Bounds.left + object2Bounds.width * object2.getScale().x;
		auto height = object2Bounds.top + object2Bounds.height * object2.getScale().y;

		sf::Vector2u position(object2Bounds.left, object2Bounds.top);

		if (!object2Bounds.intersects(object1Bounds))
			return false;

		for (auto x = position.x; x < width; ++x)
		{
			for (auto y = position.y; y < height; ++y)
			{
				auto relX = x - object1Bounds.left;

				auto relY = (object2.getCategory() & Category::EnemyProjectile) ? y - object1Bounds.top - object2Bounds.height : y - object1Bounds.top;

				if (object1.getPixel(relX, relY))
					return true;
			}
		}

		return false;
	}

	constexpr auto Padding = 40.f;
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

	bounds.top += Padding;
	bounds.height -= Padding * 2;

	bounds.left += Padding;
	bounds.width -= Padding * 2;

	return bounds;
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

	line[0].color = line[1].color = sf::Color::Green;

	mTarget.draw(line, 2, sf::Lines);
	mTarget.draw(mStaticScoreText);
	mTarget.draw(mScoreText);
	mTarget.draw(mLivesText);

#ifdef _DEBUG
	//mQuadTreePrimary.draw(mTarget);
	//mQuadTreeSecondary.draw(mTarget);
#endif

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
	addShield(  85.f, 150.f);
	addShield( -85.f, 150.f);
	addShield( 250.f, 150.f);
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
	auto colHight = 35u;

	for (auto i = 0u; i < 6u; ++i)
	{
		for (auto j = 0u; j < 6u; ++j)
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

	sf::Vector2f position = mPlayerShip->getPosition();

	position.x = std::max(position.x, viewBounds.left + Padding);
	position.x = std::min(position.x, viewBounds.left + viewBounds.width - Padding - 5.f);

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
			else if (collision(*node1, *node2))
			{
				auto& enemy = static_cast<Spaceship&>(*node2);
				auto& projectile = static_cast<Projectile&>(*node1);

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
			else if (collision(*node1, *node2))
			{

				auto& player = static_cast<Spaceship&>(*node2);
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
				auto& enemy = static_cast<Spaceship&>(*node1);

				if (PixelcollidesPair(shield, enemy))
				{
					shield.onHit(enemy.getBoundingRect(), enemy.getPosition(), enemy.getCategory());
				}
			}
			else if (collision(*node1, *node2))
			{
				auto& player = static_cast<Spaceship&>(*node2);
				auto& enemy = static_cast<Spaceship&>(*node1);

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
	return mEnemyNodes.empty();
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
		Spaceship& enemy = static_cast<Spaceship&>(*mEnemyNodes[i]);

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