#include "Shield.hpp"
#include "DataTables.hpp"
#include "Utility.hpp"
#include "ResourceHolder.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/CircleShape.hpp>

namespace
{
	constexpr auto ExplosionRadious = 10.f;
}


Shield::Shield(const ImageHolder& images, sf::Vector2u windowsize)
	: Entity(1)
	, mImage(images.get(Images::Shield))
	, mDrity(true)
	, mOnHit(false)
	, mRectOnHit()
	, mPositionOnHit()
	, mSign()
	, mRenderTexture()
	, mSprite()
	, mTexture()
	, mTransform(sf::Transform::Identity)
{
	mRenderTexture.create(windowsize.x, windowsize.y);
	mRenderTexture.clear();
	mRenderTexture.display();

	updateSprite();

}

void Shield::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (mDrity)
	{
		mTransform.combine(getTransform());
		mDrity = false;
	}
	states.transform = mTransform;
	target.draw(mSprite, states);
}

unsigned int Shield::getCategory() const
{
	return Category::Shield;
}

void Shield::onHit(sf::FloatRect rect, sf::Vector2f position, int sign)
{
	mRectOnHit = rect;
	mPositionOnHit = position;
	mOnHit = true;
	mSign = sign;
}

void Shield::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	if (!mOnHit)
		return;

	mRenderTexture.draw(*this, sf::BlendNone);
	mRenderTexture.display();

	float radious = (mRectOnHit.height > ExplosionRadious) ? mRectOnHit.height : ExplosionRadious;
	sf::CircleShape circle(radious, 32);
	circle.setPosition(mPositionOnHit.x, mPositionOnHit.y + circle.getRadius() / 2.f * mSign);
	circle.setFillColor(sf::Color::Transparent);
	centerOrigin(circle);
	mRenderTexture.draw(circle, sf::BlendNone);
	mRenderTexture.display();

	sf::Image image(mRenderTexture.getTexture().copyToImage());
	sf::Vector2u position(static_cast<std::size_t>(getPosition().x - mTexture.getSize().x / 2u), static_cast<std::size_t>(getPosition().y - mTexture.getSize().y / 2u));
	mImage.copy(image, 0u, 0u, sf::IntRect(position.x, position.y, mTexture.getSize().x, mTexture.getSize().y));
	mImage.createMaskFromColor(sf::Color::Transparent);

	updateSprite();
}

sf::FloatRect Shield::getBoundingRect() const
{
	return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

bool Shield::getPixel(std::size_t x, std::size_t y) const
{
	if (x < mImage.getSize().x && y < mImage.getSize().y)
		return mImage.getPixel(x, y) != sf::Color::Transparent;

	return false;
}

void Shield::updateSprite()
{
	mTexture.loadFromImage(mImage);
	mSprite.setTexture(mTexture);
	centerOrigin(mSprite);
	mOnHit = false;
}