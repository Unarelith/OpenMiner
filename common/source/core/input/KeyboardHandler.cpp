/*
 * =====================================================================================
 *
 *       Filename:  KeyboardHandler.cpp
 *
 *    Description:
 *
 *        Created:  11/02/2015 15:49:47
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include <gk/core/IntTypes.hpp>
#include <gk/core/XMLFile.hpp>
#include <gk/core/Debug.hpp>

#include "GameKey.hpp"
#include "KeyboardHandler.hpp"

KeyboardHandler::KeyboardHandler() {
	gk::XMLFile doc("resources/config/keys.xml");

	tinyxml2::XMLElement *keys = doc.FirstChildElement("keys").ToElement();

	// Reading keys from names as defined here: https://wiki.libsdl.org/SDL_Keycode
	auto addKey = [&](gk::GameKey key, const char *name) {
		tinyxml2::XMLElement *keyElement = keys->FirstChildElement(name);
		m_keys[key] = SDL_GetScancodeFromName(keyElement->Attribute("key"));

		if(m_keys[key] == SDL_SCANCODE_UNKNOWN) {
			DEBUG("Key '", keyElement->Attribute("key"), "' not recognized");
		}

		InputHandler::addKey(key);
	};

	addKey(GameKey::Left,      "Left");
	addKey(GameKey::Right,     "Right");
	addKey(GameKey::Up,        "Up");
	addKey(GameKey::Down,      "Down");

	addKey(GameKey::Jump,      "Jump");
	addKey(GameKey::Fly,       "Fly");
	addKey(GameKey::Sneak,     "Sneak");
	addKey(GameKey::Sprint,    "Sprint");

	addKey(GameKey::Dig,       "Dig");
	addKey(GameKey::Use,       "Use");

	addKey(GameKey::Inventory, "Inventory");
}

bool KeyboardHandler::isKeyPressed(gk::GameKey key) {
	const u8 *keyboardState = SDL_GetKeyboardState(nullptr);
	SDL_Scancode keyScancode = m_keys[key];

	m_keysPressed[key] = keyboardState[keyScancode];

	return keyboardState[keyScancode];
}

