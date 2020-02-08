/*
 * =====================================================================================
 *
 *  OpenMiner
 *  Copyright (C) 2018-2020 Unarelith, Quentin Bazin <openminer@unarelith.net>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *
 * =====================================================================================
 */
#ifndef LUAMOD_HPP_
#define LUAMOD_HPP_

#include <sol.hpp>

// This class is meant to be used ONLY in Lua
class LuaMod {
	public:
		LuaMod(const std::string &id) : m_id(id) {}

		void registerBlock(const sol::table &table);
		void registerItem(const sol::table &table);
		void registerCraftingRecipe(const sol::table &table);
		void registerSmeltingRecipe(const sol::table &table);

		const std::string &id() const { return m_id; }

		static void initUsertype(sol::state &lua);

	private:
		std::string m_id;
};

#endif // LUAMOD_HPP_
