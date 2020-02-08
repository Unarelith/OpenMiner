/*
 * =====================================================================================
 *
 *       Filename:  BlockMetadata.cpp
 *
 *    Description:
 *
 *        Created:  08/02/2020 16:03:18
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
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

