/*
 * =====================================================================================
 *
 *  OpenMiner
 *
 *  Copyright (C) 2018-2020 Unarelith, Quentin Bazin <openminer@unarelith.net>
 *  Copyright (C) 2019-2020 the OpenMiner contributors (see CONTRIBUTORS.md)
 *
 *  This file is part of OpenMiner.
 *
 *  OpenMiner is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  OpenMiner is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with OpenMiner; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *
 * =====================================================================================
 */
#include <fstream>

#include <gk/core/Debug.hpp>
#include <gk/core/Filesystem.hpp>

#include <sol/sol.hpp>

#include "GameKey.hpp"
#include "KeyboardHandler.hpp"

KeyboardHandler::KeyboardHandler() {
	addKey(GameKey::Left,            "Left",            sf::Keyboard::A);
	addKey(GameKey::Right,           "Right",           sf::Keyboard::D);
	addKey(GameKey::Up,              "Up",              sf::Keyboard::W);
	addKey(GameKey::Down,            "Down",            sf::Keyboard::S);

	addKey(GameKey::Jump,            "Jump",            sf::Keyboard::Space);
	addKey(GameKey::Fly,             "Fly",             sf::Keyboard::X);
	addKey(GameKey::Sneak,           "Sneak",           sf::Keyboard::LShift);
	addKey(GameKey::Sprint,          "Sprint",          sf::Keyboard::LControl);

	addKey(GameKey::Chat,            "Chat",            sf::Keyboard::T);
	addKey(GameKey::Command,         "Command",         sf::Keyboard::Divide);

	addKey(GameKey::Shift,           "Shift",           sf::Keyboard::LShift);
}

void KeyboardHandler::loadKeysFromFile(const std::string &filename) {
	if (gk::Filesystem::fileExists(filename)) {
		sol::state lua;

		try {
			lua.safe_script_file(filename);

			sol::optional<sol::table> keys = lua["keys"];
			if (keys != sol::nullopt) {
				for (auto &it : keys.value()) {
					const std::string &keyID = it.first.as<std::string>();
					const std::string &keyValue = it.second.as<std::string>();

					sf::Keyboard::Key keycode = gk::KeyboardUtils::getKeyFromName(keyValue);

					auto keyit = m_keysID.find(keyID);
					if (keyit != m_keysID.end()) {
						gk::GameKey key = keyit->second;
						m_keys[key].setKeycode(keycode);
						if (m_keys[key].keycode() == sf::Keyboard::Unknown) {
							gkWarning() << "Key name '" + keyValue + "' not recognized";
						}
					}
					else {
						addKey(m_keys.size(), "", keycode, keyID);
					}
				}
			}

			gkInfo() << "Key mapping loaded successfully";
		}
		catch (sol::error &e) {
			gkError() << e.what();
		}
	}
}

void KeyboardHandler::saveKeysToFile(const std::string &filename) {
	std::ofstream file{filename, std::ofstream::out | std::ofstream::trunc};
	file << "keys = {" << std::endl;
	for (auto &it : m_keys) {
		file << "\t[\"" << it.second.stringID() << "\"] = \"" << getKeyName(it.first) << "\"," << std::endl;
	}
	file << "}" << std::endl;
}

bool KeyboardHandler::isKeyPressed(gk::GameKey key) {
	return sf::Keyboard::isKeyPressed(m_keys[key].keycode());
}

void KeyboardHandler::addKey(gk::GameKey key, const std::string &name, sf::Keyboard::Key defaultKey, const std::string &stringID) {
	auto keyit = m_keysID.find(stringID);
	if (keyit == m_keysID.end()) {
		auto it = m_keys.emplace(key, Key((u16)key, (stringID.empty() ? "_" + name : stringID), name));
		it.first->second.setKeycode(defaultKey);

		m_keysID.emplace(it.first->second.stringID(), key);
	}
	else {
		m_keys.at(keyit->second).setName(name);
	}

	InputHandler::addKey(key);
}

