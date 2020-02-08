/*
 * =====================================================================================
 *
 *  OpenMiner
 *  Copyright (C) 2018-2020 Unarelith, Quentin Bazin <openminer@unarelith.net>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *
 * =====================================================================================
 */
#include <SFML/Network/Packet.hpp>

#include "BlockMetadata.hpp"

void BlockMetadata::setString(const std::string &name, const std::string &value) {
	m_data[name].set(value, BlockMetadataValue::Type::String);
}

void BlockMetadata::setInt(const std::string &name, int value) {
	m_data[name].set(value, BlockMetadataValue::Type::Int);
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
	}
}

