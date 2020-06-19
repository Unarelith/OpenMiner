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
#ifndef KEY_HPP_
#define KEY_HPP_

#include <string>

#include <gk/core/IntTypes.hpp>
#include <gk/core/input/KeyboardUtils.hpp>

#include <sol/sol.hpp>

#include "ISerializable.hpp"
#include "NetworkUtils.hpp"

#include <gk/core/Debug.hpp>

class Key : public ISerializable {
	public:
		Key() = default;
		Key(u16 id, const std::string &stringID, const std::string &name)
			: m_id(id), m_stringID(stringID), m_name(name) {}

		void serialize(sf::Packet &packet) const override {
			packet << m_id << m_stringID << m_name << m_defaultKey;
		}

		void deserialize(sf::Packet &packet) override {
			packet >> m_id >> m_stringID >> m_name >> m_defaultKey;
			m_keycode = gk::KeyboardUtils::getKeyFromName(m_defaultKey);
		}

		u16 id() const { return m_id; }

		const std::string &stringID() const { return m_stringID; }
		const std::string &name() const { return m_name; }
		void setName(const std::string &name) { m_name = name; }

		sf::Keyboard::Key keycode() const { return m_keycode; }
		void setKeycode(sf::Keyboard::Key keycode) { m_keycode = keycode; }

		const std::string &defaultKey() const { return m_defaultKey; }
		void setDefaultKey(const std::string &defaultKey) { m_defaultKey = defaultKey; }

		const sol::unsafe_function &callback() const { return m_callback; }
		void setCallback(const sol::unsafe_function &callback) { m_callback = callback; }

	private:
		u16 m_id;

		std::string m_stringID;
		std::string m_name;

		std::string m_defaultKey;
		sf::Keyboard::Key m_keycode = sf::Keyboard::Unknown;

		sol::unsafe_function m_callback;
};

#endif // KEY_HPP_
