/*
 * =====================================================================================
 *
 *       Filename:  Keyboard.cpp
 *
 *    Description:
 *
 *        Created:  20/12/2014 00:52:10
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "GameClock.hpp"
#include "Keyboard.hpp"
#include "SDLHeaders.hpp"

std::map<Keyboard::Key, bool> Keyboard::pressed = {
	{Key::D,		false},
	{Key::Q,		false},
	{Key::S,		false},
	{Key::W,		false},
	{Key::X,		false},
	{Key::Z,		false},
	{Key::E,		false},

	{Key::Left,		false},
	{Key::Right,	false},
	{Key::Up,		false},
	{Key::Down,		false},

	{Key::BackSpace,  false},
	{Key::LeftShift,  false},
	{Key::RightShift, false},
	{Key::Space,	  false},
	{Key::Return,	  false}
};

std::map<Keyboard::Key, u32> Keyboard::lastTimePressed = {
	{Key::D,		0},
	{Key::Q,		0},
	{Key::S,		0},
	{Key::W,		0},
	{Key::X,		0},
	{Key::Z,		0},
	{Key::E,		0},

	{Key::Left,		0},
	{Key::Right,	0},
	{Key::Up,		0},
	{Key::Down,		0},

	{Key::BackSpace,  0},
	{Key::LeftShift,  0},
	{Key::Space,	  0},
	{Key::Return,	  0},
	{Key::RightShift, 0}
};

std::map<Keyboard::Key, u32> Keyboard::keysCode = {
#ifdef USE_SDL
	{Key::D,		SDLK_d},
	{Key::Q,		SDLK_q},
	{Key::S,		SDLK_s},
	{Key::W,		SDLK_w},
	{Key::X,		SDLK_x},
	{Key::Z,		SDLK_z},
	{Key::E,		SDLK_e},

	{Key::Left,		SDLK_LEFT},
	{Key::Right,	SDLK_RIGHT},
	{Key::Up,		SDLK_UP},
	{Key::Down,		SDLK_DOWN},

	{Key::BackSpace,  SDLK_BACKSPACE},
	{Key::LeftShift,  SDLK_LSHIFT},
	{Key::Space,	  SDLK_SPACE},
	{Key::Return,	  SDLK_RETURN},
	{Key::RightShift, SDLK_RSHIFT}
#elif defined USE_SFML
	{Key::D,		sf::Keyboard::D},
	{Key::Q,		sf::Keyboard::Q},
	{Key::S,		sf::Keyboard::S},
	{Key::W,		sf::Keyboard::W},
	{Key::X,		sf::Keyboard::X},
	{Key::Z,		sf::Keyboard::Z},
	{Key::E,		sf::Keyboard::E},

	{Key::Left,		sf::Keyboard::Left},
	{Key::Right,	sf::Keyboard::Right},
	{Key::Up,		sf::Keyboard::Up},
	{Key::Down,		sf::Keyboard::Down},

	{Key::BackSpace,  sf::Keyboard::BackSpace},
	{Key::LeftShift,  sf::Keyboard::LShift},
	{Key::Space,	  sf::Keyboard::Space},
	{Key::Return,	  sf::Keyboard::Return},
	{Key::RightShift, sf::Keyboard::RShift}
#endif // USE_SDL, USE_SFML
};

#ifdef USE_SDL
namespace {
const u8 *getState() {
	return SDL_GetKeyboardState(nullptr);
}
}
#endif // USE_SDL

bool Keyboard::isKeyPressed(Key key) {
#ifdef USE_SDL
	if(getState()[SDL_GetScancodeFromKey(keysCode[key])]) {
		return true;
	} else {
		return false;
	}
#elif defined USE_SFML
	return sf::Keyboard::isKeyPressed(sf::Keyboard::Key(keysCode[key]));
#endif // USE_SDL, USE_SFML
}

bool Keyboard::isKeyPressedOnce(Key key) {
	if(Keyboard::isKeyPressed(key)) {
		if(!pressed[key]) {
			pressed[key] = true;
			return true;
		} else {
			return false;
		}
	} else {
		pressed[key] = false;
		return false;
	}
}

bool Keyboard::isKeyPressedWithDelay(Key key, u16 delay) {
	if(Keyboard::isKeyPressed(key) && GameClock::getTicks() - lastTimePressed[key] > delay) {
		lastTimePressed[key] = GameClock::getTicks();
		return true;
	} else {
		if(!Keyboard::isKeyPressed(key)) {
			lastTimePressed[key] = 0;
		}

		return false;
	}
}

