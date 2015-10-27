#pragma once


#include "Component.hpp"
#include "ResourceIdentifiers.hpp"
#include "State.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <functional>


class SoundPlayer;


namespace GUI
{

	class Button final : public Component
	{
	public:
		using Ptr = std::shared_ptr<Button>;


	private:
		using Callback = std::function<void()>;

		enum Type
		{
			Normal,
			Selected,
			Pressed,
			ButtonCount
		};


	public:
		explicit				Button(State::Context context);

		void					setCallback(Callback callback);
		void					setText(const std::string& text);
		void					setToggle(bool flag);

		bool					isSelectable() const override;
		void					select() override;
		void					deselect() override;

		void					activate() override;
		void					deactivate() override;

		void					handleEvent(const sf::Event&, sf::Vector2f) override;
		bool					contains(sf::Vector2f) const override;


	private:
		void					draw(sf::RenderTarget& target, sf::RenderStates states) const override;
		void					changeTexture(Type buttonType);


	private:
		Callback				mCallback;
		sf::Sprite				mSprite;
		sf::Text				mText;
		bool					mIsToggle;
		SoundPlayer&			mSounds;
		sf::RenderWindow&		mWindow;
	};
}