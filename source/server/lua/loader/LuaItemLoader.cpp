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
#include "common/core/Debug.hpp"
#include "common/core/Registry.hpp"
#include "server/core/ServerConfig.hpp"
#include "server/lua/LuaMod.hpp"
#include "server/lua/loader/LuaItemLoader.hpp"
#include "server/world/ServerItem.hpp"

void LuaItemLoader::loadItem(const sol::table &table) const {
	std::string stringID = m_mod.id() + ":" + table["id"].get<std::string>();
	std::string label = table["name"].get<std::string>();

	TilesDef tiles{stringID, 0};
	tiles.loadFromLuaTable(table);

	ServerItem &item = Registry::getInstance().registerItem<ServerItem>(tiles, stringID, label);
	item.setHarvestCapability(u8(table["harvest_capability"].get_or(0)));
	item.setMiningSpeed(table["mining_speed"].get_or(1.f));
	item.setOnItemActivated(table["on_item_activated"].get<sol::object>());
	item.setMaxStackSize(table["max_stack_size"].get_or(ServerConfig::maxItemStackSize));

	sol::object groupsObject = table["groups"].get<sol::object>();
	if (groupsObject.valid()) {
		if (groupsObject.get_type() == sol::type::table) {
			sol::table groupsTable = groupsObject.as<sol::table>();
			for (auto &groupObject : groupsTable) {
				item.addGroup("group:" + groupObject.first.as<std::string>(), groupObject.second.as<u16>());
			}
		}
		else
			logError() << "For item" << stringID << ": 'groups' should be a table";
	}

	sol::object effectiveOnObject = table["effective_on"].get<sol::object>();
	if (effectiveOnObject.valid()) {
		if (effectiveOnObject.get_type() == sol::type::table) {
			sol::table effectiveOnTable = effectiveOnObject.as<sol::table>();
			for (auto &effectiveOnObject : effectiveOnTable) {
				item.addEffectiveBlock(effectiveOnObject.second.as<std::string>());
			}
		}
		else
			logError() << "For item" << stringID << ": 'effective_on' should be a table";
	}
}
