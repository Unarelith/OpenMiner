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
#include "ItemBlock.hpp"
#include "ScriptEngine.hpp"
#include "Player.hpp"
#include "Registry.hpp"

#include <iostream>

ScriptEngine *ScriptEngine::s_instance = nullptr;

void ScriptEngine::init() {
	setInstance(this);

	m_lua.new_usertype<Player>("Player",
		"inventory", &Player::inventory);

	m_lua.new_usertype<Inventory>("Inventory",
		"width",    &Inventory::width,
		"height",   &Inventory::height,
		"getStack", &Inventory::getStackRef,
		"setStack", &Inventory::setStack,
		"addStack", &Inventory::addStack
	);

	m_lua.new_usertype<ItemStack>("ItemStack",
		"item",   sol::property(&ItemStack::item,   &ItemStack::setItem),
		"amount", sol::property(&ItemStack::amount, &ItemStack::setAmount)
	);

	m_lua.new_usertype<Item>("Item",
		"name", &Item::name,
		"id",   &Item::id
	);

	m_lua.new_usertype<Registry>("Registry",
		"registerBlock", [] (Registry *reg, u32 id, u32 textureID, const std::string &name) {
			reg->registerBlock<Block>(id, textureID, name);
		},
		"registerItemBlock", [] (Registry *reg, u32 id, const std::string &name) {
			reg->registerItem<ItemBlock>(id, id, name);
		},
		"registerItem", [] (Registry *reg, u32 id, const std::string &name, u32 textureID) {
			reg->registerItem<Item>(id, textureID, name);
		}
	);

	m_lua["Registry"] = &Registry::getInstance();

	m_lua.open_libraries();
	m_lua.script_file("mods/test.lua");
}

