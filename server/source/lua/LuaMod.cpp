/*
 * =====================================================================================
 *
 *       Filename:  LuaMod.cpp
 *
 *    Description:
 *
 *        Created:  04/01/2019 17:20:09
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include <map>

#include "CraftingRecipe.hpp"
#include "LuaMod.hpp"
#include "Registry.hpp"
#include "ServerBlock.hpp"
#include "SmeltingRecipe.hpp"

void LuaMod::registerBlock(const sol::table &table) {
	TilesDef tiles;
	tiles.loadFromLuaTable(table);

	std::string id = m_id + ":" + table["id"].get<std::string>();
	std::string name = table["name"].get<std::string>();
	sol::function onBlockActivated = table["on_block_activated"];
	sol::function onTick = table["on_tick"];

	ServerBlock &block = Registry::getInstance().registerBlock<ServerBlock>(tiles, id, name);
	block.setHarvestRequirements(table["harvest_requirements"].get_or(0));
	block.setHardness(table["hardness"].get_or(1.0f));
	block.setOpaque(table["is_opaque"].get_or(true));
	block.setLightSource(table["is_light_source"].get_or(false));
	block.setOnBlockActivated(onBlockActivated);
	block.setOnTick(onTick);

	sol::optional<sol::table> boundingBox = table["bounding_box"];
	if (boundingBox != sol::nullopt) {
		block.setBoundingBox(gk::FloatBox{
			boundingBox.value().get<float>(1),
			boundingBox.value().get<float>(2),
			boundingBox.value().get<float>(3),
			boundingBox.value().get<float>(4),
			boundingBox.value().get<float>(5),
			boundingBox.value().get<float>(6),
		});
	}

	// FIXME: Use string instead
	sol::optional<BlockDrawType> drawType = table["draw_type"];
	if (drawType != sol::nullopt) {
		block.setDrawType(drawType.value());
	}

	sol::optional<sol::table> itemDrop = table["item_drop"];
	if (itemDrop != sol::nullopt) {
		std::string dropID = itemDrop.value()["id"];
		u16 dropAmount = itemDrop.value()["amount"];
		block.setItemDrop(dropID, dropAmount);
	}

	Registry::getInstance().registerItem(block.tiles(), id, name).setIsBlock(true);
}

void LuaMod::registerItem(const sol::table &table) {
	TilesDef tiles;
	tiles.loadFromLuaTable(table);

	std::string id = table["id"].get<std::string>();
	std::string name = table["name"].get<std::string>();

	Item &item = Registry::getInstance().registerItem(tiles, id, name);
	item.setIsFuel(table["is_fuel"].get_or(false));
	item.setBurnTime(table["burn_time"].get_or(0));
	item.setHarvestCapability(table["harvest_capability"].get_or(0));
	item.setMiningSpeed(table["mining_speed"].get_or(1));
}

void LuaMod::registerCraftingRecipe(const sol::table &table) {
	sol::table resultTable = table["result"];
	sol::table patternTable = table["pattern"];
	sol::table keysTable = table["keys"];

	ItemStack result = {
		resultTable["id"].get<std::string>(),
		resultTable["amount"].get<u16>()
	};

	std::vector<std::string> pattern;
	for (auto &it : patternTable)
		pattern.emplace_back(it.second.as<std::string>());

	std::map<char, std::vector<std::string>> keys;
	for (auto &it : keysTable) {
		keys.emplace(it.first.as<char>(), std::vector<std::string>{it.second.as<std::string>()});
	}

	Registry::getInstance().registerRecipe<CraftingRecipe>(pattern, keys, result);
}

void LuaMod::registerSmeltingRecipe(const sol::table &table) {
	sol::table inputTable = table["input"];
	sol::table outputTable = table["output"];

	ItemStack input = {
		inputTable["id"].get<std::string>(),
		inputTable["amount"].get<u16>()
	};

	ItemStack output = {
		outputTable["id"].get<std::string>(),
		outputTable["amount"].get<u16>()
	};

	Registry::getInstance().registerRecipe<SmeltingRecipe>(input, output);
}

void LuaMod::initUsertype(sol::state &lua) {
	lua.new_usertype<LuaMod>("LuaMod",
		sol::constructors<LuaMod(std::string)>(),
		"id",              &LuaMod::id,
		"block",           &LuaMod::registerBlock,
		"item",            &LuaMod::registerItem,
		"crafting_recipe", &LuaMod::registerCraftingRecipe,
		"smelting_recipe", &LuaMod::registerSmeltingRecipe
	);
}

