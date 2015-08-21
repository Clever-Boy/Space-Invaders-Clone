#pragma once


#include "Component.hpp"
#include "SoundPlayer.hpp"

#include <vector>
#include <memory>


namespace GUI
{
	class Container final : public Component
	{
	public:
		using Ptr = std::shared_ptr<Container>;
		using ComponentContainer = std::vector<Component::Ptr>;


	public:
		explicit			Container(SoundPlayer& soundplayer);

		void				pack(Component::Ptr component);

		bool				isSelectable() const override;
		void				handleEvent(const sf::Event& event, sf::Vector2f) override;


	private:
		void				draw(sf::RenderTarget& target, sf::RenderStates states) const override;

		bool				hasSelection() const;
		void				select(std::size_t index);
		void				selectNext();
		void				selectPrevious();
		void				validateChild(sf::Vector2f position);


	private:
		ComponentContainer	mChildren;
		Component::Ptr		mHovered;
		int					mSelectedChild;
		SoundPlayer&		mSounds;
	};
}
