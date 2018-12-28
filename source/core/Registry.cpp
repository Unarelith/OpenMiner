/*
 * =====================================================================================
 *
 *       Filename:  Registry.cpp
 *
 *    Description:
 *
 *        Created:  23/06/2018 22:43:56
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "CraftingRecipe.hpp"
#include "ItemBlock.hpp"
#include "Registry.hpp"
#include "SmeltingRecipe.hpp"

#include "BlockFurnace.hpp"
#include "BlockWater.hpp"
#include "BlockWorkbench.hpp"
#include "XMLFile.hpp"

Registry *Registry::s_instance = nullptr;

void Registry::registerBlockFromTable(const sol::table &table) {
	u32 textureID = table["texture"].get<u32>();
	std::string name = table["name"].get<std::string>();
	std::string label = table["label"].get<std::string>();

	u32 id = m_blocks.size();
	Block *block = nullptr;
	if (id == BlockType::Workbench)    block = registerBlock<BlockWorkbench>();
	else if (id == BlockType::Furnace) block = registerBlock<BlockFurnace>();
	else if (id == BlockType::Water)   block = registerBlock<BlockWater>();
	else block = registerBlock<Block>(id, textureID, name, label);

	block->setHarvestRequirements(table["harvest_requirements"].get_or(0));
	block->setHardness(table["hardness"].get_or(1.0f));

	sol::optional<sol::table> itemDrop = table["item_drop"];
	if (itemDrop != sol::nullopt) {
		u16 id = table["id"];
		u16 amount = table["amount"];
		block->setItemDrop(id, amount);
	}

	registerItem<ItemBlock>(id, name, label);
}

void Registry::registerItemFromTable(const sol::table &table) {
	u32 textureID = table["texture"].get<u32>();
	std::string name = table["name"].get<std::string>();
	std::string label = table["label"].get<std::string>();

	Item *item = registerItem<Item>(textureID, name, label);
	item->setIsFuel(table["is_fuel"].get_or(false));
	item->setBurnTime(table["burn_time"].get_or(0));
	item->setHarvestCapability(table["harvest_capability"].get_or(0));
	item->setMiningSpeed(table["mining_speed"].get_or(1));
}

void Registry::registerCraftingRecipeFromTable(const sol::table &table) {
	sol::table resultTable = table["result"];
	sol::table patternTable = table["pattern"];
	sol::table keysTable = table["keys"];

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

	registerRecipe<CraftingRecipe>(pattern, keys, result);
}

void Registry::registerSmeltingRecipeFromTable(const sol::table &table) {
	sol::table inputTable = table["input"];
	sol::table outputTable = table["output"];

	ItemStack input = {
		inputTable["item"].get<u16>(),
		inputTable["amount"].get<u16>()
	};

	ItemStack output = {
		outputTable["item"].get<u16>(),
		outputTable["amount"].get<u16>()
	};

	registerRecipe<SmeltingRecipe>(input, output);
}

const Recipe *Registry::getRecipe(const Inventory &inventory) const {
	for (auto &recipe : m_recipes) {
		if (recipe->isMatching(inventory))
			return recipe.get();
	}
	return nullptr;
}

