#include "Button.hpp"
#include "Utility.hpp"
#include "SoundPlayer.hpp"
#include "ResourceHolder.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderTarget.hpp>


namespace GUI
{
	Button::Button(State::Context context)
		: mCallback()
		, mSprite(context.textures.get(Textures::Buttons))
		, mText("", context.fonts.get(Fonts::Main), 16)
		, mIsToggle(false)
		, mSounds(context.sounds)
		, mWindow(context.window)
	{
		changeTexture(Normal);

		auto bounds(mSprite.getLocalBounds());
		mText.setPosition(bounds.width / 2.f, bounds.height / 2.f);
	}

	void Button::setCallback(Callback callback)
	{
		mCallback = std::move(callback);
	}

	void Button::setText(const std::string& text)
	{
		using namespace utility;

		mText.setString(text);
		centerOrigin(mText);
	}

	void Button::setToggle(bool flag)
	{
		mIsToggle = flag;
	}

	bool Button::isSelectable() const
	{
		return true;
	}

	void Button::select()
	{
		Component::select();

		changeTexture(Selected);
	}

	void Button::deselect()
	{
		Component::deselect();

		changeTexture(Normal);
	}

	void Button::activate()
	{
		Component::activate();

		// If we are toggle then we should show that the button is pressed and thus "toggled".
		if (mIsToggle)
			changeTexture(Pressed);

		if (mCallback)
			mCallback();

		// If we are not a toggle then deactivate the button since we are just momentarily activated.
		if (!mIsToggle)
			deactivate();

		mSounds.play(SoundEffect::Button);
	}

	void Button::deactivate()
	{
		Component::deactivate();

		if (mIsToggle)
		{
			// Reset texture to right one depending on if we are selected or not.
			if (isSelected())
				changeTexture(Selected);
			else
				changeTexture(Normal);
		}
	}

	void Button::handleEvent(const sf::Event&, sf::Vector2f)
	{
	}

	void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform.combine(getTransform());

		target.draw(mSprite, states);
		target.draw(mText, states);
	}

	void Button::changeTexture(Type buttonType)
	{
		sf::IntRect textureRect(0, 50 * buttonType, 200, 50);
		mSprite.setTextureRect(textureRect);
	}

	bool Button::contains(const sf::Vector2f& position) const
	{
		return getTransform().transformRect(mSprite.getGlobalBounds()).contains(position);
	}
}