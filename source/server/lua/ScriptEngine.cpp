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
		sol::lib::table
	);

	m_lua["openminer"] = &m_luaCore;
}

void ScriptEngine::initUsertypes() {
	m_lua.new_usertype<Registry>("Registry",
		"get_block",  &Registry::getBlock,
		"get_item",   &Registry::getItem,
		"get_sky",    &Registry::getSky,
		"get_tree",   &Registry::getTree,
		"get_biome",  &Registry::getBiome,
		"get_recipe", &Registry::getRecipe,

		"get_block_from_string", &Registry::getBlockFromStringID,
		"get_item_from_string",  &Registry::getItemFromStringID,
		"get_sky_from_string",   &Registry::getSkyFromStringID,
		"get_tree_from_string",  &Registry::getTreeFromStringID,
		"get_biome_from_string", &Registry::getBiomeFromStringID,

		"blocks", &Registry::blocks,
		"items", &Registry::items,
		"trees", &Registry::trees,
		"biomes", &Registry::biomes,
		"dimensions", &Registry::dimensions
	);

	m_lua.new_usertype<World>("World",
		"get_block", &World::getBlock,
		"set_block", &World::setBlock,

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
		"set_block", &Chunk::setBlock,

		"get_data", &Chunk::getData,
		"set_data", &Chunk::setData,

		"get_block_data", &Chunk::getBlockData,
		"add_block_data", &Chunk::addBlockData
	);

	m_lua.new_usertype<BlockData>("BlockData",
		"inventory", &BlockData::inventory,
		"meta", &BlockData::meta,
		"use_alt_tiles", &BlockData::useAltTiles
	);

	m_lua.new_usertype<Block>("Block",
		"id", &Block::id,
		"data", &Block::data,
		"string_id", &Block::stringID,
		"label", &Block::label,
		"mod_name", &Block::modName,
		"is_opaque", &Block::isOpaque
	);

	m_lua.new_usertype<ServerBlock>("ServerBlock",
		sol::base_classes, sol::bases<Block>()
	);

	m_lua.new_usertype<Player>("Player",
		"inventory", &Player::inventory,

		"x", &Player::x,
		"y", &Player::y,
		"z", &Player::z,
		"set_position", &Player::setPosition,

		"dimension", &Player::dimension,
		"set_dimension", &Player::setDimension,

		"client_id", &Player::clientID
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
		"string_id", &Item::stringID,
		"has_group", &Item::hasGroup,
		"get_group_value", &Item::getGroupValue
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
		"set_int", &BlockMetadata::setInt,

		"get_bool", &BlockMetadata::getLuaObject<bool>,
		"set_bool", &BlockMetadata::setBool
	);

	m_lua.new_usertype<ClientInfo>("ClientInfo",
		"id", &ClientInfo::id
	);

	m_lua.new_usertype<ServerCommandHandler>("ServerCommandHandler",
		"send_player_change_dimension", &ServerCommandHandler::sendPlayerChangeDimension,
		"send_chat_message", &ServerCommandHandler::sendChatMessage
	);

	m_lua.new_usertype<ServerModLoader>("ServerModLoader",
		"register_mod", &ServerModLoader::registerMod
	);

	LuaCore::initUsertype(m_lua);
	LuaMod::initUsertype(m_lua);
	LuaGUI::initUsertype(m_lua);
}

