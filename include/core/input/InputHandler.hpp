/*
 * =====================================================================================
 *
 *       Filename:  InputHandler.hpp
 *
 *    Description:
 *
 *        Created:  17/01/2018 19:09:57
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef INPUTHANDLER_HPP_
#define INPUTHANDLER_HPP_

#include <map>

#include "GameKey.hpp"
#include "IntTypes.hpp"

class InputHandler {
	public:
		virtual ~InputHandler() = default;

		virtual bool isKeyPressed(GameKey key) { return m_keysPressed[key]; }
		virtual bool isKeyPressedOnce(GameKey key);
		virtual bool isKeyPressedWithDelay(GameKey key, u16 delay);

		const std::map<GameKey, bool> &keysPressed() const { return m_keysPressed; }

	protected:
		std::map<GameKey, bool> m_keysPressed{
			{GameKey::Left,      false},
			{GameKey::Right,     false},
			{GameKey::Up,        false},
			{GameKey::Down,      false},

			{GameKey::Jump,      false},
			{GameKey::Fly,       false},
			{GameKey::Sneak,     false},

			{GameKey::Dig,       false},
			{GameKey::Use,       false},

			{GameKey::Inventory, false},
		};

		std::map<GameKey, bool> m_keysPressedOnce{
			{GameKey::Left,      false},
			{GameKey::Right,     false},
			{GameKey::Up,        false},
			{GameKey::Down,      false},

			{GameKey::Jump,      false},
			{GameKey::Fly,       false},
			{GameKey::Sneak,     false},

			{GameKey::Dig,       false},
			{GameKey::Use,       false},

			{GameKey::Inventory, false},
		};

		std::map<GameKey, u32> m_lastTimePressed{
			{GameKey::Left,      0},
			{GameKey::Right,     0},
			{GameKey::Up,        0},
			{GameKey::Down,      0},

			{GameKey::Jump,      0},
			{GameKey::Fly,       0},
			{GameKey::Sneak,     0},

			{GameKey::Dig,       0},
			{GameKey::Use,       0},

			{GameKey::Inventory, 0},
		};
};

#endif // INPUTHANDLER_HPP_
