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
#include <iostream>

#include <SFML/Network/Packet.hpp>

#include "Block.hpp"
#include "Player.hpp"
#include "World.hpp"

Block::Block(u32 id, const TilesDef &tiles, const std::string &stringID, const std::string &label) {
	m_id = id;
	m_tiles = tiles;

	m_stringID = stringID;
	m_label = label;

	m_itemDrop = stringID;
	m_itemDropAmount = 1;
}

void Block::serialize(sf::Packet &packet) const {
	packet << u32(m_id) << m_stringID << m_label << u8(m_drawType)
		<< m_hardness << m_harvestRequirements << m_itemDrop << m_itemDropAmount << m_tiles
		<< m_boundingBox.x << m_boundingBox.y << m_boundingBox.z
		<< m_boundingBox.width << m_boundingBox.height << m_boundingBox.depth
		<< m_isLightSource << m_canUpdate << m_canBeActivated;
}

void Block::deserialize(sf::Packet &packet) {
	u32 id;
	u8 drawType;

	packet >> id >> m_stringID >> m_label >> drawType >> m_hardness
		>> m_harvestRequirements >> m_itemDrop >> m_itemDropAmount >> m_tiles
		>> m_boundingBox.x >> m_boundingBox.y >> m_boundingBox.z
		>> m_boundingBox.width >> m_boundingBox.height >> m_boundingBox.depth
		>> m_isLightSource >> m_canUpdate >> m_canBeActivated;

	m_id = id;
	m_drawType = BlockDrawType(drawType);
}

