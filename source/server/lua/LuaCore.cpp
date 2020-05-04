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
#include "LuaCore.hpp"
#include "Registry.hpp"
#include "ServerModLoader.hpp"
#include "ServerWorld.hpp"

void LuaCore::addListener(LuaEventType eventType, const sol::function &listener) {
	m_listeners.emplace(eventType, listener);
}

void LuaCore::initUsertype(sol::state &lua) {
	lua["EventType"] = lua.create_table_with(
		"OnBlockPlaced", LuaEventType::OnBlockPlaced,
		"OnBlockDigged", LuaEventType::OnBlockDigged,
		"OnBlockActivated", LuaEventType::OnBlockActivated
	);

	lua.new_usertype<LuaCore>("LuaCore",
		"add_listener", &LuaCore::addListener,

		"registry", &LuaCore::m_registry,
		"mod_loader", &LuaCore::m_modLoader
	);
}

