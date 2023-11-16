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
#include <SFML/Network/Packet.hpp>

#include "world/BlockMetadata.hpp"

void BlockMetadata::setString(const std::string &name, const std::string &value) {
	m_data[name].set(value, BlockMetadataValue::Type::String);
}

void BlockMetadata::setInt(const std::string &name, int value) {
	m_data[name].set(value, BlockMetadataValue::Type::Int);
}

void BlockMetadata::setBool(const std::string &name, bool value) {
	m_data[name].set(value, BlockMetadataValue::Type::Bool);
}

void BlockMetadata::serialize(sf::Packet &packet) const {
	packet << u32(m_data.size());
	for (auto &it : m_data) {
		packet << u8(it.second.type()) << it.first;
		if (it.second.type() == BlockMetadataValue::Type::String) {
			packet << it.second.get<std::string>();
		}
		else if (it.second.type() == BlockMetadataValue::Type::Int) {
			packet << it.second.get<int>();
		}
		else if (it.second.type() == BlockMetadataValue::Type::Bool) {
			packet << it.second.get<bool>();
		}
	}
}

void BlockMetadata::deserialize(sf::Packet &packet) {
	u32 size;
	packet >> size;
	for (u32 i = 0 ; i < size ; ++i) {
		u8 typeU8;
		std::string name;
		packet >> typeU8 >> name;

		BlockMetadataValue::Type type = BlockMetadataValue::Type(typeU8);
		if (type == BlockMetadataValue::Type::String) {
			std::string value;
			packet >> value;
			m_data[name].set(value, type);
		}
		else if (type == BlockMetadataValue::Type::Int) {
			int value;
			packet >> value;
			m_data[name].set(value, type);
		}
		else if (type == BlockMetadataValue::Type::Bool) {
			bool value;
			packet >> value;
			m_data[name].set(value, type);
		}
	}
}

// Please update 'docs/lua-api-cpp.md' if you change this
void BlockMetadata::initUsertype(sol::state &lua) {
	lua.new_usertype<BlockMetadata>("BlockMetadata",
		"get_string", &BlockMetadata::getLuaObject<std::string>,
		"set_string", &BlockMetadata::setString,

		"get_int", &BlockMetadata::getLuaObject<int>,
		"set_int", &BlockMetadata::setInt,

		"get_bool", &BlockMetadata::getLuaObject<bool>,
		"set_bool", &BlockMetadata::setBool
	);
}
