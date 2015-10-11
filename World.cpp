#include "World.hpp"
#include "SpriteNode.hpp"
#include "Boss.hpp"
#include "Player.hpp"
#include "LifeNode.hpp"
#include "Shield.hpp"
#include "ScoreNode.hpp"
#include "SoundPlayer.hpp"
#include "SoundNode.hpp"

#include <SFML/Graphics/RenderWindow.hpp>


namespace
{
	bool collision(const SceneNode& lhs, const SceneNode& rhs)
	{
		return lhs.getBoundingRect().intersects(rhs.getBoundingRect());
	}

	template<typename GameObject>
	auto collision(const Shield& shield, const GameObject& object) ->bool
	{
		auto shieldBounds(static_cast<sf::Rect<std::size_t>>(shield.getBoundingRect()));
		auto objectBounds(static_cast<sf::Rect<std::size_t>>(object.getBoundingRect()));

		auto width = objectBounds.left + objectBounds.width;
		auto height = objectBounds.top + objectBounds.height;

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

	constexpr auto Padding			= 40.f;
	constexpr auto MovementsPadding = 55.f;
}


World::World(sf::RenderTarget&	target, FontHolder& fonts, SoundPlayer& sounds)
	: mTarget(target)
	, mFonts(fonts)
	, mSounds(sounds)
	, mWorldView(target.getDefaultView())
	, mImages()
	, mTextures()
	, mSceneGraph()
	, mSceneLayers()
	, mWorldBounds(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize())
	, mSpawnPosition(mWorldView.getSize().x / 2.f, mWorldView.getSize().y / 2.f)
	, mPlayer(nullptr)
	, mBoss(nullptr)
	, mLife(nullptr)
	, mScore(nullptr)
	, mQuadTreePrimary(1, mWorldBounds)
	, mQuadTreeSecondary(1, mWorldBounds)
	, mEnemyNodes()
	, mPlayerBulletNodes()
	, mEnemyBulletNodes()
	, mInvadersController()
	, mDeadLine(getBattlefieldBounds().height + getBattlefieldBounds().top)
	, mBossTimer(sf::Time::Zero)
	, mPlayerTimer(sf::Time::Zero)
	, mBossSpawn(true)
	, mFirstSpawn(true)
	, mIsPlayerDead(false)
	, mLivesCount(3)
	, mPreviousPosition()
	, mIsGameEnded(false)
	, mEndGame(false)
{
	loadTextures();
	buildScene();

	// Prepare the view
	mWorldView.setCenter(mSpawnPosition);
}

void World::draw()
{
	mTarget.setView(mWorldView);
	mTarget.draw(mSceneGraph);
}

void World::update(sf::Time dt)
{
	// reset player velocity
	mPlayer->setVelocity(0.f, 0.f);

	// Remove useless entities
	destroyEntitiesOutsideView();

	// create Boss 
	spawnBoss(dt);

	// Update quadtree
	checkForCollision();

	// update Invaders controller: Adapt Movements 
	mInvadersController.update(mCommandQueue);

	// Forward commands to scene graph
	while (!mCommandQueue.isEmpty())
		mSceneGraph.onCommand(mCommandQueue.pop());

	// control enemy fires
	controlEnemyFire();

	// Collision detection and response (may destroy entities)
	handleCollisions();

	// Check if Player Dead
	if (checkPlayerDeath(dt))
		return;

	mSceneGraph.removeWrecks();

	// Spawn Player
	spawnPlayer();

	// Regular update step
	mSceneGraph.update(dt, mCommandQueue);

	// Adapt position (correct if outside view)
	adaptPlayerPosition();

	updateSounds();
}

bool World::hasAlivePlayer() const
{
	if (mLivesCount == 0)
		return (!mPlayer->isMarkedForRemoval());

	return (!mEndGame);
}

bool World::hasPlayerWon() const
{
	if (mEnemyNodes.empty())
		return (mBoss->isMarkedForRemoval());

	return false;
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
		auto category = (i == Space) ? Category::SceneSpaceLayer : Category::None;

		auto layer(std::make_unique<SceneNode>(category));
		mSceneLayers[i] = layer.get();

		mSceneGraph.attachChild(std::move(layer));
	}

	//Prepare the tiled background
	auto& texture(mTextures.get(Textures::Background));

	// Add the background sprite to the scene
	auto backgroundSprite(std::make_unique<SpriteNode>(SpriteNode::Background, texture));
	backgroundSprite->setDirtyFlag(false);
	mSceneLayers[Background]->attachChild(std::move(backgroundSprite));

	// add Line
	auto lineSprite(std::make_unique<SpriteNode>(SpriteNode::Line, getBattlefieldBounds()));
	lineSprite->setDirtyFlag(false);
	mSceneLayers[Background]->attachChild(std::move(lineSprite));

	// add Score
	auto score(std::make_unique<ScoreNode>(mFonts));
	mScore = score.get();
	mScore->setDirtyFlag(false);
	mSceneLayers[Background]->attachChild(std::move(score));

	// add lives
	auto life(std::make_unique<LifeNode>(Player::PlayerShip, mTextures));
	mLife = life.get();
	mLife->setDirtyFlag(false);
	mSceneLayers[Background]->attachChild(std::move(life));

	// Add player's spaceship
	auto leader(std::make_unique<Player>(Player::PlayerShip, mTextures));
	mPlayer = leader.get();
	auto bounds(getBattlefieldBounds());
	auto YPadding = bounds.top + bounds.height - Padding / 2u - mSpawnPosition.y;
	mPlayer->setPosition(mSpawnPosition + sf::Vector2f(0.f, YPadding));
	mSceneLayers[Space]->attachChild(std::move(leader));

	// Add enemy Spaceships
	addEnemies();

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

void World::addEnemies()
{
	const auto numberOfEnemies = 66u;
	const auto enemiesPerRow = 11u;
	const auto horizontalSpacing = 40.f;
	const auto verticalSpacing = 35.f;

	const sf::Vector2f positionOfTopLeft(MovementsPadding, Padding * 2.5);

	for (auto i = 0u; i < numberOfEnemies; ++i)
	{
		sf::Vector2f position(horizontalSpacing * (i % enemiesPerRow), verticalSpacing * (i / enemiesPerRow));

		if (i < 22)
			addEnemy(Invaders::Enemy1, positionOfTopLeft.x + position.x, positionOfTopLeft.y + position.y);
		else if (i >= 22 && i < 44)
			addEnemy(Invaders::Enemy2, positionOfTopLeft.x + position.x, positionOfTopLeft.y + position.y);
		else if (i >= 44)
			addEnemy(Invaders::Enemy3, positionOfTopLeft.x + position.x, positionOfTopLeft.y + position.y);
	}
}

void World::addEnemy(Invaders::Type type, float relX, float relY)
{
	auto enemy(std::make_unique<Invaders>(type, mTextures, getMovementsfieldBounds(), mInvadersController));
	enemy->setPosition(relX, relY);
	mSceneLayers[Space]->attachChild(std::move(enemy));
}

void World::controlEnemyFire()
{
	std::sort(mEnemyNodes.begin(), mEnemyNodes.end(),
		[this](const auto& lhs, const auto& rhs)
	{
		return lhs->getPosition().y > rhs->getPosition().y;
	});

	auto size = mEnemyNodes.size();

	for (auto i = 0u; i < size; ++i)
	{
		auto& enemy = static_cast<Invaders&>(*mEnemyNodes[i]);

		if (enemy.isDestroyed())
			continue;

		if (enemy.getWorldPosition().y >= mDeadLine - 20.f)
		{
			mPlayer->damage(enemy.getHitpoints());
			mIsGameEnded = true;
		}

		if (mEnemyNodes.size() <= 3)
			enemy.setMaxSpeed(enemy.getMaxSpeed() + 1.f);

		if (i < 11)
			enemy.fire();
	}
}

void World::spawnBoss(sf::Time dt)
{
	Boss::Dirction dirction;
	auto position = 0.f;

	mBossTimer += dt;

	if (mFirstSpawn && mBossTimer > sf::seconds(5.f))
	{
		auto boss(std::make_unique<Boss>(Boss::BossShip, mTextures, mWorldBounds, Boss::MovingLeft));
		mBoss = boss.get();
		mBoss->setPosition(mWorldBounds.left + mWorldBounds.width + MovementsPadding, Padding * 1.5);
		mSceneLayers[Space]->attachChild(std::move(boss));
		mBossTimer = sf::Time::Zero;
		mFirstSpawn = false;
		return;
	}

	if (mBossTimer < sf::seconds(20.f))
		return;

	if (mBossSpawn)
	{
		dirction = Boss::MovingRight;
		position = -MovementsPadding;
	}
	else
	{
		dirction = Boss::MovingLeft;
		position = mWorldBounds.left + mWorldBounds.width + MovementsPadding;
	}

	mBossSpawn = !mBossSpawn;

	// Receate Boss
	auto boss(std::make_unique<Boss>(Boss::BossShip, mTextures, mWorldBounds, dirction));
	mBoss = boss.get();
	mBoss->setPosition(position, Padding * 1.5);
	mSceneLayers[Space]->attachChild(std::move(boss));
	mBossTimer = sf::Time::Zero;
}

bool World::checkPlayerDeath(sf::Time dt)
{
	if (!mPlayer->isDestroyed() || mIsPlayerDead)
		return false;

	mPlayer->applyHitEffect(dt, mCommandQueue);
	mPlayerTimer += dt;

	if (mPlayerTimer > sf::seconds(0.25f))
	{
		mPlayer->setMarkToRemove();
		mPreviousPosition = mPlayer->getWorldPosition();
		mIsPlayerDead = true;
		if (mIsGameEnded)
			mEndGame = true;
		mPlayerTimer = sf::Time::Zero;
	}

	return true;
}

void World::spawnPlayer()
{
	if (!mIsPlayerDead)
		return;

	auto leader(std::make_unique<Player>(Player::PlayerShip, mTextures));
	mPlayer = leader.get();
	mPlayer->setPosition(mPreviousPosition);
	mSceneLayers[Space]->attachChild(std::move(leader));
	mIsPlayerDead = false;
}

void World::adaptPlayerPosition()
{
	auto Bounds(getMovementsfieldBounds());
	auto position(mPlayer->getPosition());

	position.x = std::max(position.x, Bounds.left);
	position.x = std::min(position.x, Bounds.left + Bounds.width);

	mPlayer->setPosition(position);
}

sf::FloatRect World::getViewBounds() const
{
	return sf::FloatRect(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());
}

sf::FloatRect World::getBattlefieldBounds() const
{
	auto bounds(getViewBounds());

	bounds.top += Padding;
	bounds.height -= Padding * 2;

	bounds.left += Padding;
	bounds.width -= Padding * 2;

	return bounds;
}

sf::FloatRect World::getMovementsfieldBounds() const
{
	auto bounds(getViewBounds());

	bounds.top += MovementsPadding;
	bounds.height -= MovementsPadding * 2;

	bounds.left += MovementsPadding;
	bounds.width -= MovementsPadding * 2;

	return bounds;
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
			mQuadTreePrimary.insert(&node);
		}
		else if (node.getCategory() & Category::EnemySpaceship)
		{
			mEnemyNodes.push_back(&node);
			mQuadTreePrimary.insert(&node);
		}
		else if (node.getCategory() & Category::Shield)
		{
			mQuadTreeSecondary.insert(&node);
			mQuadTreePrimary.insert(&node);
		}
		else if (node.getCategory() & Category::PlayerSpaceship)
		{
			mQuadTreeSecondary.insert(&node);
		}
		else if (node.getCategory() & Category::BossSpaceship)
		{
			mQuadTreePrimary.insert(&node);
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

				if (!collision(shield, projectile))
					continue;

				shield.onHit(projectile.getBoundingRect(), projectile.getPosition(), projectile.getCategory());
				projectile.destroy();

			}
			else if (node2->getCategory() & Category::EnemyProjectile)
			{
				if (!collision(*node1, *node2))
					continue;

				auto& playerProjectile = static_cast<Projectile&>(*node1);
				auto& enemyProjectile = static_cast<Projectile&>(*node2);

				playerProjectile.destroy();
				enemyProjectile.destroy();
			}
			else if (node2->getCategory() & Category::BossSpaceship)
			{
				if (!collision(*node1, *node2))
					continue;

				auto& enemy = static_cast<Boss&>(*node2);
				auto& projectile = static_cast<Projectile&>(*node1);

				mScore->increment(100);

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
					mScore->increment(30);
					break;
				case Invaders::Enemy2:
					mScore->increment(20);
					break;
				case Invaders::Enemy3:
					mScore->increment(10);
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

				if (!collision(shield, projectile))
					continue;

				shield.onHit(projectile.getBoundingRect(), projectile.getPosition(), projectile.getCategory());
				projectile.destroy();

			}
			else if (node2->getCategory() & Category::PlayerSpaceship)
			{
				if (!collision(*node1, *node2))
					continue;

				auto& player = static_cast<Player&>(*node2);
				auto& projectile = static_cast<Projectile&>(*node1);

				player.damage(projectile.getDamage());

				projectile.destroy();

				mLife->decrement();
				mLivesCount--;
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

				if (collision(shield, enemy))
					shield.onHit(enemy.getBoundingRect(), enemy.getPosition(), enemy.getCategory());
			}
			else if (node2->getCategory() & Category::PlayerSpaceship)
			{
				if (!collision(*node1, *node2))
					continue;

				auto& player = static_cast<Player&>(*node2);
				auto& enemy = static_cast<Invaders&>(*node1);

				player.damage(enemy.getHitpoints());

				enemy.destroy();

				mLife->decrement();
				mLivesCount--;
			}
		}
	}
}

void World::updateSounds()
{
	mSounds.setListenerPosition(mPlayer->getWorldPosition());

	mSounds.removeStoppedSounds();
}