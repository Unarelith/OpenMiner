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
#include "Biome.hpp"
#include "LuaMod.hpp"
#include "PlacementEntry.hpp"
#include "Registry.hpp"
#include "Tree.hpp"

void LuaMod::initUsertype(sol::state &lua) {
	lua.new_usertype<LuaMod>("LuaMod",
		sol::constructors<LuaMod(std::string)>(),
		"id",              &LuaMod::id,
		"block",           &LuaMod::registerBlock,
		"item",            &LuaMod::registerItem,
		"crafting_recipe", &LuaMod::registerCraftingRecipe,
		"smelting_recipe", &LuaMod::registerSmeltingRecipe,
		"sky",             &LuaMod::registerSky,
		"tree",            &LuaMod::registerTree,
		"biome",           &LuaMod::registerBiome,
		"dimension",       &LuaMod::registerDimension
	);
}

void LuaMod::registerBlock(const sol::table &table) {
	m_blockLoader.loadBlock(table);
}

void LuaMod::registerItem(const sol::table &table) {
	m_itemLoader.loadItem(table);
}

void LuaMod::registerCraftingRecipe(const sol::table &table) {
	m_recipeLoader.loadCraftingRecipe(table);
}

void LuaMod::registerSmeltingRecipe(const sol::table &table) {
	m_recipeLoader.loadSmeltingRecipe(table);
}

void LuaMod::registerSky(const sol::table &table) {
	m_skyLoader.loadSky(table);
}

void LuaMod::registerTree(const sol::table &table) {
	m_biomeLoader.loadTree(table);
}

void LuaMod::registerBiome(const sol::table &table) {
	m_biomeLoader.loadBiome(table);
}

void LuaMod::registerDimension(const sol::table &table) {
	m_dimensionLoader.loadDimension(table);
}

