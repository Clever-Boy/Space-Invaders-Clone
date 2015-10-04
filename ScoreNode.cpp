#include "ScoreNode.hpp"
#include "Utility.hpp"
#include "ResourceHolder.hpp"

#include <SFML/Graphics/RenderTarget.hpp>


ScoreNode::ScoreNode(const FontHolder& fonts)
	: mScore()
	, mStaticScoreText()
	, mScoreText()
	, mLivesText()
{
	using namespace utility;

	const auto TextPadding = 5.f;
	mStaticScoreText.setString("Score: ");
	mStaticScoreText.setFont(fonts.get(Fonts::Main));
	mStaticScoreText.setPosition(TextPadding, TextPadding);
	mStaticScoreText.setCharacterSize(30u);

	mScoreText.setString(std::to_string(mScore));
	mScoreText.setFont(fonts.get(Fonts::Main));
	auto bounds(mStaticScoreText.getGlobalBounds());
	mScoreText.setPosition(bounds.left + bounds.width + TextPadding, TextPadding * TextPadding);
	mScoreText.setCharacterSize(25u);
	mScoreText.setColor(sf::Color::Green);
	centerOrigin(mScoreText);

	mLivesText.setString("Lives: ");
	mLivesText.setFont(fonts.get(Fonts::Main));
	const auto XPadding = 600.f;
	mLivesText.setPosition(XPadding, TextPadding);
	mLivesText.setCharacterSize(30u);
}

void ScoreNode::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	mScoreText.setString(std::to_string(mScore));
}

void ScoreNode::increment(std::size_t score)
{
	mScore += score;
}

void ScoreNode::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mStaticScoreText, states);
	target.draw(mScoreText, states);
	target.draw(mLivesText, states);
}