#include "TitleState.hpp"
#include "Utility.hpp"
#include "ResourceHolder.hpp"

#include <SFML/Graphics/RenderWindow.hpp>


TitleState::TitleState(StateStack& stack, Context context)
	: State(stack, context)
	, mText()
	, mShowText(true)
	, mTextEffectTime(sf::Time::Zero)
{
	using namespace utility;

	const auto Padding = 150.f;

	mBackgroundSprite.setTexture(context.textures.get(Textures::TitleScreen));
	mBackgroundSprite.setScale(1.25f, 1.f);
	mText.setFont(context.fonts.get(Fonts::Main));
	mText.setString("Press any key to start");
	centerOrigin(mText);
	sf::Vector2f position(context.window.getSize() / 2u);
	mText.setPosition(position.x, position.y + Padding);
}

void TitleState::draw()
{
	auto& window = getContext().window;
	window.draw(mBackgroundSprite);

	if (mShowText)
		window.draw(mText);
}

bool TitleState::update(sf::Time dt)
{
	const auto EffectInterval = 0.5f;

	mTextEffectTime += dt;

	if (mTextEffectTime >= sf::seconds(EffectInterval))
	{
		mShowText = !mShowText;
		mTextEffectTime = sf::Time::Zero;
	}

	return true;
}

bool TitleState::handleEvent(const sf::Event& event)
{
	// If any key is pressed, trigger the next screen
	if (event.type == sf::Event::KeyReleased)
	{
		requestStackPop();
		requestStackPush(States::Menu);
	}

	return true;
}