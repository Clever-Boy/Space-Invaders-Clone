#pragma once


#include "ResourceIdentifiers.hpp"
#include "SceneNode.hpp"

#include <SFML/Graphics/Text.hpp>


class ScoreNode final : public SceneNode
{
public:
	explicit 				ScoreNode(const FontHolder& fonts);

	void					increment(std::size_t score);


private:
	void 					draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void 					updateCurrent(sf::Time dt, CommandQueue& commands) override;


private:
	sf::Text				mScoreText;
	sf::Text				mStaticScoreText;
	sf::Text				mLivesText;
	std::size_t				mScore;
};