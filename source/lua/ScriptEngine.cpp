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
#include "ScriptEngine.hpp"
#include "Player.hpp"
#include "Registry.hpp"

#include "CraftingRecipe.hpp"
#include "SmeltingRecipe.hpp"

#include "BlockFurnace.hpp"
#include "BlockWater.hpp"
#include "BlockWorkbench.hpp"

ScriptEngine *ScriptEngine::s_instance = nullptr;

void ScriptEngine::init() {
	setInstance(this);

	initUsertypes();

	m_lua["registry"] = &Registry::getInstance();

	m_lua.open_libraries(sol::lib::base);
	m_lua.safe_script_file("mods/test.lua");
}

void ScriptEngine::initUsertypes() {
	m_lua.new_usertype<Player>("Player",
		"inventory", &Player::inventory);

	m_lua.new_usertype<Inventory>("Inventory",
		"get_width",  &Inventory::width,
		"get_height", &Inventory::height,
		"get_stack",  &Inventory::getStackRef,
		"set_stack",  &Inventory::setStack,
		"add_stack",  &Inventory::addStack
	);

	m_lua.new_usertype<ItemStack>("ItemStack",
		"item",   sol::property(&ItemStack::item,   &ItemStack::setItem),
		"amount", sol::property(&ItemStack::amount, &ItemStack::setAmount)
	);

	m_lua.new_usertype<Item>("Item",
		"id",                 &Item::id,
		"label",              &Item::label,
		"is_fuel",            sol::property(&Item::isFuel, &Item::setIsFuel),
		"burn_time",          sol::property(&Item::burnTime, &Item::setBurnTime),
		"harvest_capability", sol::property(&Item::harvestCapability, &Item::setHarvestCapability),
		"mining_speed",       sol::property(&Item::miningSpeed, &Item::setMiningSpeed)
	);

	m_lua.new_usertype<Block>("Block",
		"id",                   &Block::id,
		"data",                 &Block::data,
		"label",                sol::property(&Block::label, &Block::setLabel),
		"harvest_requirements", sol::property(&Block::harvestRequirements, &Block::setHarvestRequirements),
		"hardness",             sol::property(&Block::hardness, &Block::setHardness),
		"set_item_drop",        &Block::setItemDrop
	);

	m_lua.new_usertype<Registry>("Registry",
		"register_block", &Registry::registerBlockFromTable,
		"register_item", &Registry::registerItemFromTable,
		"register_crafting_recipe", &Registry::registerCraftingRecipeFromTable,
		"register_smelting_recipe", &Registry::registerSmeltingRecipeFromTable
	);
}

