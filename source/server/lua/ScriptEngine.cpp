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
#include "BlockMetadata.hpp"
#include "EntityWrapper.hpp"
#include "LuaCore.hpp"
#include "LuaGUI.hpp"
#include "LuaMod.hpp"
#include "Registry.hpp"
#include "ScriptEngine.hpp"
#include "ServerBlock.hpp"
#include "ServerCommandHandler.hpp"
#include "ServerModLoader.hpp"
#include "ServerPlayer.hpp"
#include "ServerWorld.hpp"

void ScriptEngine::init() {
	initUsertypes();

	// Note: To be safe, don't add sol::lib::io and provide a better way to load/save data
	//       With a feature like this, it would be way easier to add io restrictions
	m_lua.open_libraries(
		sol::lib::base,
		sol::lib::math,
		sol::lib::table,
		sol::lib::string
	);

	m_lua["openminer"] = &m_luaCore;
}

void ScriptEngine::initUsertypes() {
	Block               ::initUsertype(m_lua);
	BlockMetadata       ::initUsertype(m_lua);
	BlockParam          ::initUsertype(m_lua);
	BlockState          ::initUsertype(m_lua);
	Chunk               ::initUsertype(m_lua);
	Dimension           ::initUsertype(m_lua);
	EntityWrapper       ::initUsertype(m_lua);
	Inventory           ::initUsertype(m_lua);
	Item                ::initUsertype(m_lua);
	ItemStack           ::initUsertype(m_lua);
	Player              ::initUsertype(m_lua);
	Registry            ::initUsertype(m_lua);
	ServerBlock         ::initUsertype(m_lua);
	ServerCommandHandler::initUsertype(m_lua);
	ServerModLoader     ::initUsertype(m_lua);
	ServerPlayer        ::initUsertype(m_lua);
	World               ::initUsertype(m_lua);
	ServerWorld         ::initUsertype(m_lua);

	m_lua.new_usertype<BlockData>("BlockData",
		"inventory", &BlockData::inventory,
		"meta", &BlockData::meta
	);

	m_lua.new_usertype<Recipe>("Recipe",
		"type", &Recipe::type,
		"result", &Recipe::result
	);

	m_lua.new_usertype<glm::ivec3>("ivec3",
		"x", &glm::ivec3::x,
		"y", &glm::ivec3::y,
		"z", &glm::ivec3::z
	);

	m_lua.new_usertype<ClientInfo>("ClientInfo",
		"id", &ClientInfo::id,
		"screen_width", &ClientInfo::screenWidth,
		"screen_height", &ClientInfo::screenHeight,
		"gui_scale", &ClientInfo::guiScale
	);

	LuaCore::initUsertype(m_lua);
	LuaMod::initUsertype(m_lua);
	LuaGUI::initUsertype(m_lua);
}

