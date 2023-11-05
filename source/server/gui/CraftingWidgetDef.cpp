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
#include "CraftingWidgetDef.hpp"
#include "Debug.hpp"

void CraftingWidgetDef::serialize(sf::Packet &packet) const {
	WidgetDef::serialize(packet);

	packet << m_resultX << m_resultY << m_shiftDestination << m_inventory;

	if (m_inventory == "block")
		packet << m_blockPosition << m_offset << m_size;
	else if (m_inventory == "temp")
		packet << m_size;
}

void CraftingWidgetDef::loadFromLuaTable(const sol::table &table) {
	WidgetDef::loadFromLuaTable(table);

	loadInventory(table);

	m_shiftDestination = table["shift_destination"].get_or<std::string>("");

	sol::optional<sol::table> resultPosTable = table["result_pos"];
	if (resultPosTable != sol::nullopt) {
		m_resultX = resultPosTable.value()["x"];
		m_resultY = resultPosTable.value()["y"];
	}
}

void CraftingWidgetDef::loadInventory(const sol::table &table) {
	sol::object inventoryObject = table["inventory"].get<sol::object>();
	if (inventoryObject.valid() && inventoryObject.get_type() == sol::type::table) {
		sol::table inventoryTable = inventoryObject.as<sol::table>();

		m_inventory = inventoryTable["source"].get<std::string>();
		if (m_inventory == "block") {
			sol::optional<sol::table> blockTable = inventoryTable["block"];
			if (blockTable != sol::nullopt) {
				m_blockPosition.x = blockTable.value()["x"];
				m_blockPosition.y = blockTable.value()["y"];
				m_blockPosition.z = blockTable.value()["z"];
			}
			else
				logError() << "For" << m_name << ": Block position must be defined";

			m_offset = inventoryTable["offset"].get_or<u16>(0);
			m_size = inventoryTable["size"].get_or<u16>(3);
		}
		else if (m_inventory == "temp") {
			m_size = inventoryTable["size"].get_or<u16>(3);
		}
		else {
			logError() << "For" << m_name << ": Inventory source" << m_inventory + "is not valid";
		}
	}
	else
		logError() << "For" << m_name << ": 'inventory' field must be a table";
}

