/*
 * =====================================================================================
 *
 *       Filename:  Keyboard.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  20/12/2014 00:52:10
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Quentin BAZIN, <quent42340@gmail.com>
 *        Company:  
 *
 * =====================================================================================
 */
#include "GameClock.hpp"
#include "Keyboard.hpp"
#include "SDLHeaders.hpp"

std::map<Keyboard::Key, bool> Keyboard::pressed = {
	{Key::Left,		false},
	{Key::Right,	false},
	{Key::Up,		false},
	{Key::Down,		false},
	
	{Key::D,		false},
	{Key::Q,		false},
	{Key::S,		false},
	{Key::W,		false},
	{Key::X,		false},
	{Key::Z,		false},
	
	{Key::BackSpace,false},
	{Key::Return,	false}
};

std::map<Keyboard::Key, u32> Keyboard::lastTimePressed = {
	{Key::Left,		0},
	{Key::Right,	0},
	{Key::Up,		0},
	{Key::Down,		0},
	
	{Key::D,		0},
	{Key::Q,		0},
	{Key::S,		0},
	{Key::W,		0},
	{Key::X,		0},
	{Key::Z,		0},
	
	{Key::BackSpace,0},
	{Key::Return,	0}
};

std::map<Keyboard::Key, u32> Keyboard::keysCode = {
	{Key::Left,		SDLK_LEFT},
	{Key::Right,	SDLK_RIGHT},
	{Key::Up,		SDLK_UP},
	{Key::Down,		SDLK_DOWN},
	
	{Key::D,		SDLK_d},
	{Key::Q,		SDLK_q},
	{Key::S,		SDLK_s},
	{Key::W,		SDLK_w},
	{Key::X,		SDLK_x},
	{Key::Z,		SDLK_z},
	
	{Key::BackSpace,SDLK_BACKSPACE},
	{Key::Return,	SDLK_RETURN}
};

const u8 *Keyboard::getState() {
	return SDL_GetKeyboardState(nullptr);
}

bool Keyboard::isKeyPressed(Key key) {
	if(getState()[SDL_GetScancodeFromKey(keysCode[key])]) {
		return true;
	} else {
		return false;
	}
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

