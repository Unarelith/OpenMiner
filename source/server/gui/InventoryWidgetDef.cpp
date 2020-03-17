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
#include <gk/core/Debug.hpp>

#include "InventoryWidgetDef.hpp"

void InventoryWidgetDef::serialize(sf::Packet &packet) const {
	WidgetDef::serialize(packet);

	packet << m_width << m_height << m_shiftDestination << m_offset << m_count << m_inventory;

	if (m_inventory == "player")
		packet << m_player << m_inventoryName;
	else if (m_inventory == "temp")
		packet << m_inventoryName;
	else if (m_inventory == "block")
		packet << m_blockPosition;
}

void InventoryWidgetDef::loadFromLuaTable(const sol::table &table) {
	WidgetDef::loadFromLuaTable(table);

	loadInventory(table);

	m_shiftDestination = table["shift_destination"].get_or<std::string>("");

	sol::optional<sol::table> size = table["size"];
	if (size != sol::nullopt) {
		m_width = size.value()["x"];
		m_height = size.value()["y"];
	}
	else
		DEBUG("ERROR: For '" + m_name + "': Inventory size must be defined");
}

void InventoryWidgetDef::loadInventory(const sol::table &table) {
	sol::object inventoryObject = table["inventory"];
	if (inventoryObject.valid() && inventoryObject.get_type() == sol::type::table) {
		sol::table inventoryTable = inventoryObject.as<sol::table>();

		m_offset = inventoryTable["offset"].get_or<u16>(0);
		m_count = inventoryTable["count"].get<u16>();

		m_inventory = inventoryTable["source"].get<std::string>();
		if (m_inventory == "player") {
			m_player = inventoryTable["player"].get<std::string>();
			m_inventoryName = inventoryTable["inventory_name"].get<std::string>();
		}
		else if (m_inventory == "temp") {
			m_inventoryName = inventoryTable["inventory_name"].get_or<std::string>("");
		}
		else if (m_inventory == "block") {
			sol::optional<sol::table> blockTable = inventoryTable["block"];
			if (blockTable != sol::nullopt) {
				m_blockPosition.x = blockTable.value()["x"];
				m_blockPosition.y = blockTable.value()["y"];
				m_blockPosition.z = blockTable.value()["z"];
			}
			else
				DEBUG("ERROR: For '" + m_name + "': Block position must be defined");
		}
		else {
			DEBUG("ERROR: For '" + m_name + "': Inventory source '" + m_inventory + "' is not valid");
		}
	}
	else
		DEBUG("ERROR: For '" + m_name + "': 'inventory' field must be a table");
}

