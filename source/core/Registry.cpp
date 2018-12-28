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

	Block &block = registerBlock(textureID, name, label);
	block.setHarvestRequirements(table["harvest_requirements"].get_or(0));
	block.setHardness(table["hardness"].get_or(1.0f));

	sol::optional<sol::table> itemDrop = table["item_drop"];
	if (itemDrop != sol::nullopt) {
		std::string dropName = itemDrop.value()["name"];
		u16 dropAmount = itemDrop.value()["amount"];
		block.setItemDrop(dropName, dropAmount);
	}

	registerItem(block.textureID(), name, label).setIsBlock(true);
}

void Registry::registerItemFromTable(const sol::table &table) {
	u32 textureID = table["texture"].get<u32>();
	std::string name = table["name"].get<std::string>();
	std::string label = table["label"].get<std::string>();

	Item &item = registerItem(textureID, name, label);
	item.setIsFuel(table["is_fuel"].get_or(false));
	item.setBurnTime(table["burn_time"].get_or(0));
	item.setHarvestCapability(table["harvest_capability"].get_or(0));
	item.setMiningSpeed(table["mining_speed"].get_or(1));
}

void Registry::registerCraftingRecipeFromTable(const sol::table &table) {
	sol::table resultTable = table["result"];
	sol::table patternTable = table["pattern"];
	sol::table keysTable = table["keys"];

	ItemStack result = {
		resultTable["item"].get<std::string>(),
		resultTable["amount"].get<u16>()
	};

	std::vector<std::string> pattern;
	for (auto &it : patternTable)
		pattern.emplace_back(it.second.as<std::string>());

	std::map<char, std::vector<std::string>> keys;
	for (auto &it : keysTable) {
		keys.emplace(it.first.as<char>(), std::vector<std::string>{it.second.as<std::string>()});
	}

	registerRecipe<CraftingRecipe>(pattern, keys, result);
}

void Registry::registerSmeltingRecipeFromTable(const sol::table &table) {
	sol::table inputTable = table["input"];
	sol::table outputTable = table["output"];

	ItemStack input = {
		inputTable["item"].get<std::string>(),
		inputTable["amount"].get<u16>()
	};

	ItemStack output = {
		outputTable["item"].get<std::string>(),
		outputTable["amount"].get<u16>()
	};

	registerRecipe<SmeltingRecipe>(input, output);
}

const Block &Registry::getBlock(const std::string &name) {
	if (name.empty()) return getBlock((int)0);
	auto it = m_blocksID.find(name);
	if (it == m_blocksID.end())
		throw EXCEPTION("Unknown block:", name);
	return getBlock(it->second);
}

const Item &Registry::getItem(const std::string &name) {
	if (name.empty()) return getItem((int)0);
	auto it = m_itemsID.find(name);
	if (it == m_itemsID.end())
		throw EXCEPTION("Unknown item:", name);
	return getItem(it->second);
}

const Recipe *Registry::getRecipe(const Inventory &inventory) const {
	for (auto &recipe : m_recipes) {
		if (recipe->isMatching(inventory))
			return recipe.get();
	}
	return nullptr;
}

Block &Registry::registerBlock(u32 textureID, const std::string &name, const std::string &label) {
	u32 id = m_blocks.size();
	m_blocksID.emplace(name, id);
	return m_blocks.emplace_back(id, textureID, name, label);
}

Item &Registry::registerItem(u32 textureID, const std::string &name, const std::string &label) {
	u32 id = m_items.size();
	m_itemsID.emplace(name, id);
	return m_items.emplace_back(id, textureID, name, label);
}

