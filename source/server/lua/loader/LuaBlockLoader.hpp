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
#ifndef LUABLOCKLOADER_HPP_
#define LUABLOCKLOADER_HPP_

#include <sol.hpp>

class Item;
class LuaMod;
class ServerBlock;

class LuaBlockLoader {
	public:
		LuaBlockLoader(LuaMod &mod) : m_mod(mod) {}

		void loadBlock(const sol::table &table) const;

	private:
		void loadProperties(ServerBlock &block, const sol::table &table) const;
		void loadBoundingBox(ServerBlock &block, const sol::table &table) const;
		void loadDrawType(ServerBlock &block, const sol::table &table) const;
		void loadItemDrop(ServerBlock &block, const sol::table &table) const;
		void loadColorMultiplier(ServerBlock &block, const sol::table &table) const;

		void loadGroups(ServerBlock &block, Item &item, const sol::table &table) const;

		LuaMod &m_mod;
};

#endif // LUABLOCKLOADER_HPP_
