#include "SoundNode.hpp"
#include "SoundPlayer.hpp"


SoundNode::SoundNode(SoundPlayer& player)
	: SceneNode()
	, mSounds(player)
{
}

void SoundNode::playSound(SoundEffect::ID sound, sf::Vector2f position)
{
	mSounds.play(sound, position);
}

void SoundNode::playRepeatedSound(sf::Vector2f position)
{
	mSounds.playRepeatedSound(position);
}

void SoundNode::stopRepeatedSound()
{
	mSounds.stopRepeatedSound();
}

unsigned int SoundNode::getCategory() const
{
	return Category::SoundEffect;
}