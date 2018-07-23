/*
 * =====================================================================================
 *
 *       Filename:  Keyboard.hpp
 *
 *    Description:
 *
 *        Created:  20/12/2014 00:52:04
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef KEYBOARD_HPP_
#define KEYBOARD_HPP_

#include <map>

#include "IntTypes.hpp"

class Keyboard {
	public:
		enum Key {
			D,
			Q,
			S,
			W,
			X,
			Z,
			E,

			Left,
			Right,
			Up,
			Down,

			BackSpace,
			LeftShift,
			Space,
			Return,
			RightShift
		};

		static bool isKeyPressed(Key key);
		static bool isKeyPressedOnce(Key key);
		static bool isKeyPressedWithDelay(Key key, u16 delay);

	private:
    #ifdef USE_SDL
		static const u8 *getState();
    #endif // USE_SDL

		static std::map<Key, bool> pressed;
		static std::map<Key, u32> lastTimePressed;
		static std::map<Key, u32> keysCode;
};

#endif // KEYBOARD_HPP_
