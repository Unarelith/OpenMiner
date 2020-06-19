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
#ifndef KEYBOARDHANDLER_HPP_
#define KEYBOARDHANDLER_HPP_

#include <map>
#include <string>

#include <gk/core/input/InputHandler.hpp>
#include <gk/core/input/KeyboardUtils.hpp>

#include "Key.hpp"

class KeyboardHandler : public gk::InputHandler {
	public:
		KeyboardHandler();

		void loadKeysFromFile(const std::string &filename);
		void saveKeysToFile(const std::string &filename);

		bool isKeyPressed(gk::GameKey key) override;

		sf::Keyboard::Key getKeycode(gk::GameKey key) { return m_keys[key].keycode(); }
		std::string getKeyName(gk::GameKey key) { return gk::KeyboardUtils::getNameFromKey(m_keys[key].keycode()); }
		void setKeycode(gk::GameKey key, sf::Keyboard::Key keycode) { m_keys[key].setKeycode(keycode); }

		void addKey(gk::GameKey id, const std::string &name, sf::Keyboard::Key defaultKey, const std::string &stringID = "", Key *key = nullptr);
		u32 keyCount() { return m_keys.size(); }

		const std::map<gk::GameKey, Key> &keys() const { return m_keys; }

	protected:
		std::map<gk::GameKey, Key> m_keys;
		std::map<std::string, gk::GameKey> m_keysID;
};

#endif // KEYBOARDHANDLER_HPP_
