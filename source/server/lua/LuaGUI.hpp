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
#ifndef LUAGUI_HPP_
#define LUAGUI_HPP_

#include <list>

#include "Inventory.hpp"
#include "WidgetDef.hpp"

class ClientInfo;

// This class is meant to be used ONLY in Lua
class LuaGUI {
	public:
		void addImage(const sol::table &table);
		void addTextButton(const sol::table &table);
		void addInventoryWidget(const sol::table &table);
		void addCraftingWidget(const sol::table &table);
		void addProgressBarWidget(const sol::table &table);
		void addScrollBarWidget(const sol::table &table);
		void addInventory(const sol::table &table);

		void setSize(u16 width, u16 height) { m_width = width; m_height = height; }
		void setCentered(bool isCentered) { m_isCentered = isCentered; }

		void show(ClientInfo &client);

		static void initUsertype(sol::state &lua);

	private:
		u16 m_width = 0;
		u16 m_height = 0;

		bool m_isCentered = false;

		std::list<Inventory> m_inventoryList;
		std::list<std::unique_ptr<WidgetDef>> m_widgetDefinitions;
};

#endif // LUAGUI_HPP_
