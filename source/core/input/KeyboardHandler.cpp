/*
 * =====================================================================================
 *
 *       Filename:  KeyboardHandler.cpp
 *
 *    Description:
 *
 *        Created:  17/01/2018 19:14:12
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "KeyboardHandler.hpp"

void KeyboardHandler::resetState() {
	for(auto &it : m_keysPressed) {
		it.second = false;
	}
}

void KeyboardHandler::updateState(const sf::Event &event) {
	for(auto &it : m_keys) {
		if(event.type == sf::Event::KeyPressed && event.key.code == it.second) {
			m_keysPressed[it.first] = true;
		}

		if (event.type == sf::Event::KeyReleased && event.key.code == it.second) {
			m_keysPressed[it.first] = false;
		}
	}
}

