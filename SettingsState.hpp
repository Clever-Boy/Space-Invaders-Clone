#pragma once


#include "State.hpp"
#include "PlayerController.hpp"
#include "Container.hpp"
#include "Button.hpp"
#include "Label.hpp"

#include <SFML/Graphics/Sprite.hpp>

#include <array>


class SettingsState final : public State
{
	using BindingButtons = std::array<GUI::Button::Ptr, PlayerController::ActionCount>;
	using BindingLabels = std::array<GUI::Label::Ptr, PlayerController::ActionCount>;


public:
	explicit						SettingsState(StateStack& stack, Context context);

	void							draw() override;
	bool							update(sf::Time dt) override;
	bool							handleEvent(const sf::Event& event) override;


private:
	void							updateLabels();
	void							addButtonLabel(PlayerController::Action action, float y, const std::string& text, Context context);


private:
	sf::Sprite						mBackgroundSprite;
	GUI::Container					mGUIContainer;
	BindingButtons					mBindingButtons;
	BindingLabels 					mBindingLabels;
};