#include "SettingsState.hpp"
#include "Utility.hpp"
#include "ResourceHolder.hpp"

#include <SFML/Graphics/RenderWindow.hpp>


SettingsState::SettingsState(StateStack& stack, Context context)
	: State(stack, context)
	, mGUIContainer(context.sounds)
{
	mBackgroundSprite.setTexture(context.textures.get(Textures::TitleScreen));
	mBackgroundSprite.setScale(1.25f, 1.f);

	// Build key binding buttons and labels
	addButtonLabel(PlayerController::MoveLeft, 300.f, "Move Left", context);
	addButtonLabel(PlayerController::MoveRight, 350.f, "Move Right", context);
	addButtonLabel(PlayerController::Fire, 450.f, "Fire", context);

	updateLabels();

	auto backButton(std::make_shared<GUI::Button>(context));
	backButton->setPosition(80.f, 540.f);
	backButton->setText("Back");
	backButton->setCallback(std::bind(&SettingsState::requestStackPop, this));

	mGUIContainer.pack(backButton);
}

void SettingsState::draw()
{
	auto& window(getContext().window);

	window.clear();
	window.draw(mBackgroundSprite);
	window.draw(mGUIContainer);
}

bool SettingsState::update(sf::Time)
{
	return true;
}

bool SettingsState::handleEvent(const sf::Event& event)
{
	bool isKeyBinding = false;

	// Iterate through all key binding buttons to see if they are being pressed, waiting for the user to enter a key
	for (auto action = 0u; action < PlayerController::ActionCount; ++action)
	{
		if (mBindingButtons[action]->isActive())
		{
			isKeyBinding = true;

			if (event.type == sf::Event::KeyReleased)
			{
				auto& player(getContext().playerController);
				player.assignKey(static_cast<PlayerController::Action>(action), event.key.code);
				mBindingButtons[action]->deactivate();
			}

			break;
		}
	}

	// If pressed button changed key bindings, update labels; otherwise consider other buttons in container
	if (isKeyBinding)
	{
		updateLabels();
	}
	else
	{
		const auto& window(getContext().window);
		auto position(window.mapPixelToCoords(sf::Mouse::getPosition(window)));

		mGUIContainer.handleEvent(event, position);
	}

	return false;
}

void SettingsState::updateLabels()
{
	using namespace utility;

	auto& player(getContext().playerController);

	for (auto i = 0u; i < PlayerController::ActionCount; ++i)
	{
		auto key(player.getAssignedKey(static_cast<PlayerController::Action>(i)));
		mBindingLabels[i]->setText(toString(key));
	}
}

void SettingsState::addButtonLabel(PlayerController::Action action, float y, const std::string& text, Context context)
{
	mBindingButtons[action] = std::make_shared<GUI::Button>(context);
	mBindingButtons[action]->setPosition(80.f, y);
	mBindingButtons[action]->setText(text);
	mBindingButtons[action]->setToggle(true);

	mBindingLabels[action] = std::make_shared<GUI::Label>("", context.fonts);
	mBindingLabels[action]->setPosition(300.f, y + 15.f);

	mGUIContainer.pack(mBindingButtons[action]);
	mGUIContainer.pack(mBindingLabels[action]);
}