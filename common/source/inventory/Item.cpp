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

