/*
 * =====================================================================================
 *
 *       Filename:  GamePad.hpp
 *
 *    Description:
 *
 *        Created:  17/01/2018 19:11:54
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef GAMEPAD_HPP_
#define GAMEPAD_HPP_

#include "InputHandler.hpp"

class GamePad {
	public:
		static void init(InputHandler &_inputHandler) { inputHandler = &_inputHandler; }

		static bool isKeyPressed(GameKey key);
		static bool isKeyPressedOnce(GameKey key);
		static bool isKeyPressedWithDelay(GameKey key, u16 delay);

		static InputHandler *getInputHandler() { return inputHandler; }

	private:
		static InputHandler *inputHandler;
};

#endif // GAMEPAD_HPP_
