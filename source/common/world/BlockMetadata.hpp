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
#ifndef BLOCKMETADATA_HPP_
#define BLOCKMETADATA_HPP_

#include <memory>
#include <string>
#include <unordered_map>

#include <sol/sol.hpp>

#include <gk/core/IntTypes.hpp>

#include "Exception.hpp"
#include "ISerializable.hpp"

class BlockMetadataValue {
	public:
		enum class Type : u8 {
			Undefined,
			String,
			Int,
			Bool
		};

		template<typename T>
		T &get() const {
			return *std::static_pointer_cast<T>(m_value);
		}

		template<typename T>
		void set(const T &value, Type type) {
			m_value = std::make_shared<T>(value);
			m_type = type;
		}

		Type type() const { return m_type; }

	private:
		std::shared_ptr<void> m_value;
		Type m_type = Type::Undefined;
};

class BlockMetadata : public ISerializable {
	public:
		void setString(const std::string &name, const std::string &value);
		void setInt(const std::string &name, int value);
		void setBool(const std::string &name, bool value);

		template<typename T>
		T &get(const std::string &name) {
			auto it = m_data.find(name);
			if (it == m_data.end())
				throw EXCEPTION("Unable to find metadata named", name);

			return it->second.get<T>();
		}

		template<typename T>
		sol::object getLuaObject(const std::string &name, sol::this_state state) {
			sol::state_view lua{state};

			auto it = m_data.find(name);
			if (it == m_data.end())
				return sol::make_object(lua, sol::lua_nil);

			return sol::make_object<T>(lua, it->second.get<T>());
		}

		void serialize(sf::Packet &packet) const override;
		void deserialize(sf::Packet &packet) override;

		static void initUsertype(sol::state &lua);

	private:
		std::unordered_map<std::string, BlockMetadataValue> m_data;
};

#endif // BLOCKMETADATA_HPP_
