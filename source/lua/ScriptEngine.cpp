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

#include "CraftingRecipe.hpp"
#include "SmeltingRecipe.hpp"

#include "BlockFurnace.hpp"
#include "BlockWater.hpp"
#include "BlockWorkbench.hpp"

ScriptEngine *ScriptEngine::s_instance = nullptr;

void ScriptEngine::init() {
	setInstance(this);

	initUsertypes();

	m_lua["Registry"] = &Registry::getInstance();

	m_lua.open_libraries(sol::lib::base);
	m_lua.safe_script_file("mods/test.lua");
}

void ScriptEngine::initUsertypes() {
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
		"name",              &Item::name,
		"id",                &Item::id,
		"isFuel",            sol::property(&Item::isFuel, &Item::setIsFuel),
		"burnTime",          sol::property(&Item::burnTime, &Item::setBurnTime),
		"harvestCapability", sol::property(&Item::harvestCapability, &Item::setHarvestCapability),
		"miningSpeed",       sol::property(&Item::miningSpeed, &Item::setMiningSpeed)
	);

	m_lua.new_usertype<Block>("Block",
		"id",                  &Block::id,
		"data",                &Block::data,
		"name",                sol::property(&Block::name, &Block::setName),
		"harvestRequirements", sol::property(&Block::harvestRequirements, &Block::setHarvestRequirements),
		"hardness",            sol::property(&Block::hardness, &Block::setHardness),
		"setItemDrop",         &Block::setItemDrop
	);

	m_lua.new_usertype<Registry>("Registry",
		"registerBlock", [] (Registry *reg, u32 id, u32 textureID, const std::string &name)
		-> Block*
		{
			if (id == BlockType::Workbench)    return reg->registerBlock<BlockWorkbench>();
			else if (id == BlockType::Furnace) return reg->registerBlock<BlockFurnace>();
			else if (id == BlockType::Water)   return reg->registerBlock<BlockWater>();
			else return reg->registerBlock<Block>(id, textureID, name);
		},
		"registerItemBlock", [] (Registry *reg, u32 id, const std::string &name) {
			return reg->registerItem<ItemBlock>(id, id, name);
		},
		"registerItem", [] (Registry *reg, u32 id, const std::string &name, u32 textureID) {
			return reg->registerItem<Item>(id, textureID, name);
		},
		"registerCraftingRecipe", [] (Registry *reg, const sol::table &recipeDefinition) {
			sol::table resultTable = recipeDefinition["result"];
			sol::table patternTable = recipeDefinition["pattern"];
			sol::table keysTable = recipeDefinition["keys"];

			ItemStack result = {
				resultTable["item"].get<u16>(),
				resultTable["amount"].get<u16>()
			};

			std::vector<std::string> pattern;
			for (auto &it : patternTable)
				pattern.emplace_back(it.second.as<std::string>());

			std::map<char, std::vector<u32>> keys;
			for (auto &it : keysTable) {
				keys.emplace(it.first.as<char>(), std::vector<u32>{it.second.as<u32>()});
			}

			reg->registerRecipe<CraftingRecipe>(pattern, keys, result);
		},
		"registerSmeltingRecipe", [] (Registry *reg, const sol::table &recipeDefinition) {
			sol::table inputTable = recipeDefinition["input"];
			sol::table outputTable = recipeDefinition["output"];

			ItemStack input = {
				inputTable["item"].get<u16>(),
				inputTable["amount"].get<u16>()
			};

			ItemStack output = {
				outputTable["item"].get<u16>(),
				outputTable["amount"].get<u16>()
			};

			reg->registerRecipe<SmeltingRecipe>(input, output);
		}
	);
}

