/*
 * =====================================================================================
 *
 *       Filename:  Item.cpp
 *
 *    Description:
 *
 *        Created:  23/06/2018 22:30:25
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include <SFML/Network/Packet.hpp>

#include "Item.hpp"

Item::Item(u32 id, const TilesDef &tiles, const std::string &name, const std::string &label) {
	m_id = id;
	m_tiles = tiles;

	m_name = name;
	m_label = label;
}

void Item::serialize(sf::Packet &packet) const {
	packet << m_id << m_name << m_label << m_isBlock << m_isFuel
		<< m_burnTime << m_miningSpeed << m_harvestCapability << m_tiles;
}

void Item::deserialize(sf::Packet &packet) {
	packet >> m_id >> m_name >> m_label >> m_isBlock >> m_isFuel
		>> m_burnTime >> m_miningSpeed >> m_harvestCapability >> m_tiles;
}

