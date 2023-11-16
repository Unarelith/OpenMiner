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
#include <map>

#include "common/core/Registry.hpp"
#include "common/inventory/CraftingRecipe.hpp"
#include "common/inventory/SmeltingRecipe.hpp"
#include "server/lua/loader/LuaRecipeLoader.hpp"

void LuaRecipeLoader::loadCraftingRecipe(const sol::table &table) const {
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

void LuaRecipeLoader::loadSmeltingRecipe(const sol::table &table) const {
	sol::table inputTable = table["input"];
	sol::table outputTable = table["output"];

	std::string inputID = inputTable["id"].get<std::string>();
	u16 inputAmount = inputTable["amount"].get<u16>();

	ItemStack output = {
		outputTable["id"].get<std::string>(),
		outputTable["amount"].get<u16>()
	};

	Registry::getInstance().registerRecipe<SmeltingRecipe>(inputID, inputAmount, output);
}
