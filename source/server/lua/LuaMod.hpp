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
#ifndef LUAMOD_HPP_
#define LUAMOD_HPP_

#include "LuaBiomeLoader.hpp"
#include "LuaBlockLoader.hpp"
#include "LuaDimensionLoader.hpp"
#include "LuaItemLoader.hpp"
#include "LuaRecipeLoader.hpp"
#include "LuaSkyLoader.hpp"

// This class is meant to be used ONLY in Lua
class LuaMod {
	public:
		// FIXME: Check if this name has already been used
		//        Check if this name matches [a-zA-Z0-9]+
		//        Check if this name != "group"
		LuaMod(const std::string &id) : m_id(id) {}

		const std::string &id() const { return m_id; }

		static void initUsertype(sol::state &lua);

	private:
		void registerBlock(const sol::table &table);
		void registerItem(const sol::table &table);
		void registerCraftingRecipe(const sol::table &table);
		void registerSmeltingRecipe(const sol::table &table);
		void registerSky(const sol::table &table);
		void registerTree(const sol::table &table);
		void registerBiome(const sol::table &table);
		void registerDimension(const sol::table &table);

		std::string m_id;

		LuaBlockLoader m_blockLoader{*this};
		LuaItemLoader m_itemLoader{*this};
		LuaRecipeLoader m_recipeLoader;
		LuaSkyLoader m_skyLoader{*this};
		LuaBiomeLoader m_biomeLoader{*this};
		LuaDimensionLoader m_dimensionLoader{*this};
};

#endif // LUAMOD_HPP_
