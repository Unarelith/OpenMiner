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

#include "network/NetworkUtils.hpp"
#include "world/Biome.hpp"

Biome::Biome(u16 id, const std::string &stringID, const std::string &label) {
	m_id = id;
	m_stringID = stringID;
	m_label = label;
}

void Biome::serialize(sf::Packet &packet) const {
	packet << m_id << m_stringID << m_label << m_params
		<< m_topBlockID << m_groundBlockID << m_deepBlockID << m_beachBlockID << m_liquidBlockID
		<< m_portalBlockID << m_portalFrameBlockID
		<< m_flora << m_ores << m_trees;
}

void Biome::deserialize(sf::Packet &packet) {
	packet >> m_id >> m_stringID >> m_label >> m_params
		>> m_topBlockID >> m_groundBlockID >> m_deepBlockID >> m_beachBlockID >> m_liquidBlockID
		>> m_portalBlockID >> m_portalFrameBlockID
		>> m_flora >> m_ores >> m_trees;
}
