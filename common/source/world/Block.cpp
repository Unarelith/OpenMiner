/*
 * =====================================================================================
 *
 *       Filename:  Block.cpp
 *
 *    Description:
 *
 *        Created:  29/12/2014 04:56:17
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include <iostream>

#include <SFML/Network/Packet.hpp>

#include "Block.hpp"
#include "Player.hpp"
#include "World.hpp"

Block::Block(u32 id, const TilesDef &tiles, const std::string &name, const std::string &label) {
	m_id = id;
	m_tiles = tiles;

	m_name = name;
	m_label = label;

	m_itemDrop = name;
	m_itemDropAmount = 1;
}

void Block::serialize(sf::Packet &packet) {
	packet << u32(m_id) << m_name << m_label << u8(m_drawType)
		<< m_hardness << m_harvestRequirements << m_itemDrop << m_itemDropAmount;

	m_tiles.serialize(packet);
}

void Block::deserialize(sf::Packet &packet) {
	u32 id;
	u8 drawType;

	packet >> id >> m_name >> m_label >> drawType >> m_hardness
		>> m_harvestRequirements >> m_itemDrop >> m_itemDropAmount;

	m_tiles.deserialize(packet);

	m_id = id;
	m_drawType = BlockDrawType(drawType);
}

