#include "Shield.hpp"
#include "DataTables.hpp"
#include "Utility.hpp"
#include "ResourceHolder.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/CircleShape.hpp>


Shield::Shield(const ImageHolder& images, sf::Vector2u windowsize)
	: Entity(1)
	, mSprite()
	, mImage(images.get(Images::Shield))
	, mRenderTexture()
	, mTexture()
	, mSign()
	, mRectOnHit()
	, mPositionOnHit()
	, mIsHit(false)
{
	mRenderTexture.create(windowsize.x, windowsize.y);
	mRenderTexture.clear();
	mRenderTexture.display();

	updateSprite();
}

unsigned int Shield::getCategory() const
{
	return Category::Shield;
}

sf::FloatRect Shield::getBoundingRect() const
{
	return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

void Shield::onHit(sf::FloatRect rect, sf::Vector2f position, unsigned int category)
{
	mRectOnHit = rect;
	mPositionOnHit = position;
	mIsHit = true;
	mSign = (category & Category::PlayerProjectile) ? 1 : -1;
}

bool Shield::getPixel(std::size_t x, std::size_t y) const
{
	if (x < mImage.getSize().x && y < mImage.getSize().y)
		return mImage.getPixel(x, y) != sf::Color::Transparent;

	return false;
}

void Shield::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mSprite, states);
}

void Shield::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	if (!mIsHit)
		return;

	using namespace utility;

	mRenderTexture.clear();

	mRenderTexture.draw(*this, sf::BlendNone);
	mRenderTexture.display();

	auto radius = std::sqrtf(mRectOnHit.width * mRectOnHit.width + mRectOnHit.height * mRectOnHit.height);
	sf::CircleShape circle(radius, 10);
	circle.setPosition(mPositionOnHit.x, mPositionOnHit.y + circle.getRadius() / 2.f * mSign);
	circle.setFillColor(sf::Color::Transparent);
	centerOrigin(circle);

	mRenderTexture.draw(circle, sf::BlendNone);
	mRenderTexture.display();

	auto x = static_cast<std::size_t>(getPosition().x - mTexture.getSize().x / 2u);
	auto y = static_cast<std::size_t>(getPosition().y - mTexture.getSize().y / 2u);
	auto bounds(sf::IntRect(x, y, mTexture.getSize().x, mTexture.getSize().y));

	mImage.copy(mRenderTexture.getTexture().copyToImage(), 0u, 0u, bounds);
	mImage.createMaskFromColor(sf::Color::Transparent);

	updateSprite();
}

void Shield::updateSprite()
{
	using namespace utility;

	mTexture.loadFromImage(mImage);
	mSprite.setTexture(mTexture);
	centerOrigin(mSprite);
	mIsHit = false;
}