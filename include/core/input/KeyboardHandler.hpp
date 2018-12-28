/*
 * =====================================================================================
 *
 *       Filename:  KeyboardHandler.hpp
 *
 *    Description:
 *
 *        Created:  11/02/2015 15:49:54
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef KEYBOARDHANDLER_HPP_
#define KEYBOARDHANDLER_HPP_

#include <map>

#include "InputHandler.hpp"
#include "SDLHeaders.hpp"

class KeyboardHandler : public InputHandler {
	public:
		KeyboardHandler();

		bool isKeyPressed(GameKey key);

	private:
		std::map<GameKey, SDL_Keycode> m_keys;
};

#endif // KEYBOARDHANDLER_HPP_
