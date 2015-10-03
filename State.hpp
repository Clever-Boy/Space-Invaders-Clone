#pragma once


#include "StateIdentifiers.hpp"
#include "ResourceIdentifiers.hpp"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

#include <memory>


namespace sf
{
	class RenderWindow;
}

class StateStack;
class PlayerController;
class MusicPlayer;
class SoundPlayer;


class State : private sf::NonCopyable
{
public:
	using Ptr = std::unique_ptr<State>;

	struct Context
	{
		explicit			Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts, PlayerController& playerController, MusicPlayer& music, SoundPlayer& sounds);

		sf::RenderWindow&	window;
		TextureHolder&		textures;
		FontHolder&			fonts;
		PlayerController&	playerController;
		MusicPlayer&		music;
		SoundPlayer&		sounds;
	};


public:
	explicit				State(StateStack& stack, Context context);
	virtual					~State() = default;

	virtual void			draw() = 0;
	virtual bool			update(sf::Time dt) = 0;
	virtual bool			handleEvent(const sf::Event& event) = 0;


protected:
	void					requestStackPush(States::ID stateID);
	void					requestStackPop();
	void					requestStateClear();

	Context					getContext() const;


private:
	StateStack&				mStack;
	Context					mContext;
};