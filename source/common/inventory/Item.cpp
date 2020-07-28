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
#include "Item.hpp"
#include "NetworkUtils.hpp"

Item::Item(u32 id, const TilesDef &tiles, const std::string &stringID, const std::string &label) {
	m_id = id;
	m_tiles = tiles;

	m_stringID = stringID;
	m_label = label;
}

void Item::serialize(sf::Packet &packet) const {
	packet << m_id << m_tiles << m_stringID << m_label << m_isBlock
		<< m_miningSpeed << m_harvestCapability << m_groups << m_canBeActivated
		<< m_effectiveOn << m_maxStackSize;
}

void Item::deserialize(sf::Packet &packet) {
	packet >> m_id >> m_tiles >> m_stringID >> m_label >> m_isBlock
		>> m_miningSpeed >> m_harvestCapability >> m_groups >> m_canBeActivated
		>> m_effectiveOn >> m_maxStackSize;
}

// Please update 'docs/lua-api-cpp.md' if you change this
void Item::initUsertype(sol::state &lua) {
	lua["Capability"] = lua.create_table_with(
		"Pickaxe", 1,
		"Shovel", 2,
		"Axe", 4
	);

	lua.new_usertype<Item>("Item",
		"id", &Item::id,
		"string_id", &Item::stringID,
		"has_group", &Item::hasGroup,
		"get_group_value", &Item::getGroupValue
	);
}

