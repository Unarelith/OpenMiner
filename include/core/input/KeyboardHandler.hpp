/*
 * =====================================================================================
 *
 *       Filename:  KeyboardHandler.hpp
 *
 *    Description:
 *
 *        Created:  17/01/2018 19:14:03
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef KEYBOARDHANDLER_HPP_
#define KEYBOARDHANDLER_HPP_

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "InputHandler.hpp"

class KeyboardHandler : public InputHandler {
	public:
		void resetState();

		void updateState(const sf::Event &event);

	private:
		std::map<GameKey, sf::Keyboard::Key> m_keys{
			{GameKey::Left,      sf::Keyboard::Q},
			{GameKey::Right,     sf::Keyboard::D},
			{GameKey::Up,        sf::Keyboard::Z},
			{GameKey::Down,      sf::Keyboard::S},

			{GameKey::Jump,      sf::Keyboard::Space},
			{GameKey::Fly,       sf::Keyboard::X},
			{GameKey::Sneak,     sf::Keyboard::LShift},

			{GameKey::Dig,       sf::Keyboard::L},
			{GameKey::Use,       sf::Keyboard::M},

			{GameKey::Inventory, sf::Keyboard::E},
		};
};

#endif // KEYBOARDHANDLER_HPP_
