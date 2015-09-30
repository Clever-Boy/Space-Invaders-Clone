#pragma once


#include "SceneNode.hpp"
#include "ResourceIdentifiers.hpp"


class SoundPlayer;

class SoundNode final : public SceneNode
{
public:
	explicit				SoundNode(SoundPlayer& player);

	void					playSound(SoundEffect::ID sound, sf::Vector2f position);
	unsigned int			getCategory() const override;


private:
	SoundPlayer&			mSounds;
};