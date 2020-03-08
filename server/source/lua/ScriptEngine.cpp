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
#include "LuaCore.hpp"
#include "LuaGUI.hpp"
#include "LuaMod.hpp"
#include "Registry.hpp"
#include "ScriptEngine.hpp"
#include "ServerCommandHandler.hpp"
#include "ServerPlayer.hpp"
#include "ServerWorld.hpp"

ScriptEngine *ScriptEngine::s_instance = nullptr;

void ScriptEngine::init() {
	setInstance(this);

	initUsertypes();

	// Note: To be safe, don't add sol::lib::io and provide a better way to load/save data
	//       With a feature like this, it would be way easier to add io restrictions
	m_lua.open_libraries(
		sol::lib::base,
		sol::lib::math,
		sol::lib::table
	);
}

void ScriptEngine::initUsertypes() {
	m_lua.new_usertype<Registry>("Registry",
		"get_recipe", &Registry::getRecipe
	);

	m_lua.new_usertype<World>("World",
		"get_block", &World::getBlock,
		"get_data", &World::getData,
		"set_data", &World::setData,
		"get_block_data", &World::getBlockData,
		"add_block_data", &World::addBlockData
	);

	m_lua.new_usertype<ServerWorld>("ServerWorld",
		sol::base_classes, sol::bases<World>()
	);

	m_lua.new_usertype<Chunk>("Chunk",
		"get_block", &Chunk::getBlock,
		"get_data", &Chunk::getData,
		"get_block_data", &Chunk::getBlockData,
		"add_block_data", &Chunk::addBlockData
	);

	m_lua.new_usertype<BlockData>("BlockData",
		"inventory", &BlockData::inventory,
		"meta", &BlockData::meta,
		"useAltTiles", &BlockData::useAltTiles
	);

	m_lua.new_usertype<Player>("Player",
		"inventory", &Player::inventory,

		"dimension", &Player::dimension,
		"set_dimension", &Player::setDimension
	);

	m_lua.new_usertype<ServerPlayer>("ServerPlayer",
		sol::base_classes, sol::bases<Player>()
	);

	m_lua.new_usertype<Inventory>("Inventory",
		"add_stack", sol::overload(&Inventory::addStack, &Inventory::addStack2),
		"get_stack", &Inventory::getStack,
		"set_stack", &Inventory::setStack
	);

	m_lua.new_usertype<Recipe>("Recipe",
		"type", &Recipe::type,
		"result", &Recipe::result
	);

	m_lua.new_usertype<ItemStack>("ItemStack",
		"amount", &ItemStack::amount,
		"item", &ItemStack::item
	);

	m_lua.new_usertype<Item>("Item",
		"id", &Item::id,
		"stringID", &Item::stringID,
		"burn_time", &Item::burnTime,
		"is_fuel", &Item::isFuel
	);

	m_lua.new_usertype<glm::ivec3>("ivec3",
		"x", &glm::ivec3::x,
		"y", &glm::ivec3::y,
		"z", &glm::ivec3::z
	);

	m_lua.new_usertype<BlockMetadata>("BlockMetadata",
		"get_string", &BlockMetadata::getLuaObject<std::string>,
		"set_string", &BlockMetadata::setString,

		"get_int", &BlockMetadata::getLuaObject<int>,
		"set_int", &BlockMetadata::setInt
	);

	m_lua.new_usertype<ClientInfo>("ClientInfo",
		"id", &ClientInfo::id
	);

	m_lua.new_usertype<ServerCommandHandler>("ServerCommandHandler",
		"send_player_change_dimension", &ServerCommandHandler::sendPlayerChangeDimension
	);

	LuaCore::initUsertype(m_lua);
	LuaMod::initUsertype(m_lua);
	LuaGUI::initUsertype(m_lua);
}

