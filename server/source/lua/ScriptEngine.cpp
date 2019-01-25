/*
 * =====================================================================================
 *
 *       Filename:  ScriptEngine.cpp
 *
 *    Description:
 *
 *        Created:  20/12/2018 00:42:55
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "LuaCore.hpp"
#include "LuaGUI.hpp"
#include "LuaMod.hpp"
#include "Registry.hpp"
#include "ScriptEngine.hpp"
#include "ServerPlayer.hpp"
#include "ServerWorld.hpp"

ScriptEngine *ScriptEngine::s_instance = nullptr;

void ScriptEngine::init() {
	setInstance(this);

	initUsertypes();

	// FIXME: Remove these lines when they're not needed anymore
	m_lua["registry"] = &Registry::getInstance();

	// Note: To be safe, don't add sol::lib::io and provide a better way to load/save data
	//       With a feature like this, it would be way easier to add io restrictions
	m_lua.open_libraries(
		sol::lib::base,
		sol::lib::math,
		sol::lib::table
	);

	m_lua.safe_script_file("mods/test.lua");
}

void ScriptEngine::initUsertypes() {
	m_lua.new_usertype<Registry>("Registry",
		"get_recipe", &Registry::getRecipe
	);

	m_lua.new_usertype<ServerWorld>("World",
		"get_block", &ServerWorld::getBlock
		// "get_data", &ServerWorld::getData,
		// "set_data", &ServerWorld::setData,
		// "get_block_data", &ServerWorld::getBlockData
	);

	m_lua.new_usertype<ServerChunk>("Chunk",
		"get_block", &Chunk::getBlock,
		"get_data", &Chunk::getData
		// "get_block_data", &Chunk::getBlockData
	);

	m_lua.new_usertype<BlockData>("BlockData",
		"inventory", &BlockData::inventory,
		"data", &BlockData::data
	);

	m_lua.new_usertype<ServerPlayer>("Player",
		"inventory", &ServerPlayer::inventory
	);

	m_lua.new_usertype<Inventory>("Inventory",
		"add_stack", &Inventory::addStack,
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
		"name", &Item::name,
		"burn_time", &Item::burnTime
	);

	m_lua.new_usertype<glm::ivec3>("ivec3",
		"x", &glm::ivec3::x,
		"y", &glm::ivec3::y,
		"z", &glm::ivec3::z
	);

	LuaCore::initUsertype(m_lua);
	LuaMod::initUsertype(m_lua);
	LuaGUI::initUsertype(m_lua);
}

