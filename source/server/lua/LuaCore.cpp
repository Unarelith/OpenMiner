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
#include "common/core/Registry.hpp"
#include "server/core/ServerConfig.hpp"
#include "server/lua/LuaCore.hpp"
#include "server/lua/ServerModLoader.hpp"

void LuaCore::addListener(LuaEventType eventType, const sol::function &listener) {
	m_listeners.emplace(eventType, listener);
}

void LuaCore::initUsertype(sol::state &lua) {
	lua["Event"] = lua.create_table_with(
		"BlockPlaced", LuaEventType::BlockPlaced,
		"BlockDigged", LuaEventType::BlockDigged,
		"BlockActivated", LuaEventType::BlockActivated,

		"ItemActivated", LuaEventType::ItemActivated,

		"PlayerConnected", LuaEventType::PlayerConnected
	);

	lua.new_usertype<LuaCore>("LuaCore",
		"registry", &LuaCore::m_registry,
		"mod_loader", &LuaCore::m_modLoader,

		"add_listener", &LuaCore::addListener,
		"get_config", [&](const std::string &option) {
			auto it = ServerConfig::options.find(option);
			if (it == ServerConfig::options.end()) {
				logWarning() << "Option" << option << "doesn't exist";
				return sol::object{};
			}

			return it->second;
		}
	);
}
