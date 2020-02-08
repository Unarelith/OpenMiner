/*
 * =====================================================================================
 *
 *       Filename:  BlockMetadata.hpp
 *
 *    Description:
 *
 *        Created:  08/02/2020 16:02:50
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef BLOCKMETADATA_HPP_
#define BLOCKMETADATA_HPP_

#include <memory>
#include <string>
#include <unordered_map>

#include <sol.hpp>

#include <gk/core/IntTypes.hpp>
#include <gk/core/Exception.hpp>

#include "ISerializable.hpp"

class BlockMetadataValue {
	public:
		enum class Type : u8 {
			Undefined,
			String,
			Int
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

	private:
		std::unordered_map<std::string, BlockMetadataValue> m_data;
};

#endif // BLOCKMETADATA_HPP_
