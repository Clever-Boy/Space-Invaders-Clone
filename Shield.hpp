#pragma once


#include "Entity.hpp"

#include "ResourceIdentifiers.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <SFML/Graphics/RenderTexture.hpp>


class Shield final : public Entity
{
public:
	explicit				Shield(const ImageHolder& images, sf::Vector2u windowsize);

	unsigned int			getCategory() const override;
	sf::FloatRect			getBoundingRect() const override;

	void					onHit(sf::FloatRect rect, sf::Vector2f position, Category::Type category);
	bool					getPixel(std::size_t x, std::size_t y) const;


private:
	void					drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
	void 					updateCurrent(sf::Time dt, CommandQueue& commands) override;
	void					updateSprite();


private:
	sf::Sprite				mSprite;
	sf::Image				mImage;
	sf::Texture				mTexture;

	sf::RenderTexture		mRenderTexture;
	int						mSign;
	sf::FloatRect			mRectOnHit;
	sf::Vector2f			mPositionOnHit;
	bool					mOnHit;

	mutable bool			mDrity;
	mutable sf::Transform	mTransform;
};

