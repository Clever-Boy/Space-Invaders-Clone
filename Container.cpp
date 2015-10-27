#include "Container.hpp"
#include "SoundPlayer.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderTarget.hpp>


GUI::Container::Container(SoundPlayer& soundplayer)
	: mChildren()
	, mSelectedChild(-1)
	, mSounds(soundplayer)
	, mHovered(nullptr)
{
}

void GUI::Container::pack(Component::Ptr component)
{
	mChildren.push_back(std::move(component));

	if (!hasSelection() && mChildren.back()->isSelectable())
		select(mChildren.size() - 1);
}

bool GUI::Container::isSelectable() const
{
	return false;
}

void GUI::Container::handleEvent(const sf::Event& event, sf::Vector2f position)
{
	if (hasSelection() && mChildren[mSelectedChild]->isActive())
	{
		mChildren[mSelectedChild]->handleEvent(event, position);
	}
	else if (event.type == sf::Event::KeyReleased)
	{
		if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up)
		{
			selectPrevious();
			mSounds.play(SoundEffect::Button);
		}
		else if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down)
		{
			selectNext();
			mSounds.play(SoundEffect::Button);
		}
		else if (event.key.code == sf::Keyboard::Return || event.key.code == sf::Keyboard::Space)
		{
			if (hasSelection())
				mChildren[mSelectedChild]->activate();
		}
	}
	else if (event.type == sf::Event::MouseMoved)
	{
		validateChild(position);

		if (mChildren[mSelectedChild] != nullptr && mChildren[mSelectedChild] != mHovered)
		{
			mHovered = mChildren[mSelectedChild];

			if (mChildren[mSelectedChild]->contains(position))
				mSounds.play(SoundEffect::Button);
		}
		else if (mChildren[mSelectedChild] == nullptr && mHovered != nullptr)
		{
			mHovered = nullptr;
		}
	}
	else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
	{
		if (mChildren[mSelectedChild]->contains(position))
			mChildren[mSelectedChild]->activate();
	}
}

void GUI::Container::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& child : mChildren)
		target.draw(*child, states);
}

bool GUI::Container::hasSelection() const
{
	return mSelectedChild >= 0;
}

void GUI::Container::select(std::size_t index)
{
	if (mChildren[index]->isSelectable())
	{
		if (hasSelection())
			mChildren[mSelectedChild]->deselect();

		mChildren[index]->select();
		mSelectedChild = index;
	}
}

void GUI::Container::selectNext()
{
	if (!hasSelection())
		return;

	// Search next component that is selectable, wrap around if necessary
	auto next = mSelectedChild;

	do
		next = (next + 1) % mChildren.size();
	while (!mChildren[next]->isSelectable());

	// Select that component
	select(next);
}

void GUI::Container::selectPrevious()
{
	if (!hasSelection())
		return;

	// Search previous component that is selectable, wrap around if necessary
	auto prev = mSelectedChild;

	do
		prev = (prev + mChildren.size() - 1) % mChildren.size();
	while (!mChildren[prev]->isSelectable());

	// Select that component
	select(prev);
}

void GUI::Container::validateChild(sf::Vector2f position)
{
	for (auto i = 0u, size = mChildren.size(); i < size; ++i)
	{
		if (mChildren[i]->isSelectable() && mChildren[i]->contains(position))
			select(i);
	}
}