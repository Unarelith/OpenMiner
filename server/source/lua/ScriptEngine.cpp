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

	m_lua.safe_script_file("mods/default/init.lua");
}

void ScriptEngine::initUsertypes() {
	m_lua.new_usertype<Registry>("Registry",
		"get_recipe", &Registry::getRecipe
	);

	m_lua.new_usertype<World>("World",
		"get_block", &World::getBlock,
		"get_data", &World::getData,
		"set_data", &World::setData,
		"get_block_data", &World::getBlockData
	);

	m_lua.new_usertype<Chunk>("Chunk",
		"get_block", &Chunk::getBlock,
		"get_data", &Chunk::getData,
		"get_block_data", &Chunk::getBlockData
	);

	m_lua.new_usertype<BlockData>("BlockData",
		"inventory", &BlockData::inventory,
		"data", &BlockData::data
	);

	m_lua.new_usertype<Player>("Player",
		"inventory", &Player::inventory
	);

	m_lua.new_usertype<ServerPlayer>("ServerPlayer",
		sol::base_classes, sol::bases<Player>()
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

