#pragma once


#include "Component.hpp"
#include "ResourceIdentifiers.hpp"
#include "ResourceHolder.hpp"

#include <SFML/Graphics/Text.hpp>


namespace GUI
{
	class Label final : public Component
	{
	public:
		using Ptr = std::shared_ptr<Label>;


	public:
		explicit			Label(const std::string& text, const FontHolder& fonts);

		bool				isSelectable() const override;
		void				setText(const std::string& text);

		void				handleEvent(const sf::Event&, sf::Vector2f) override;


	private:
		void				draw(sf::RenderTarget& target, sf::RenderStates states) const;


	private:
		sf::Text			mText;
	};
}
