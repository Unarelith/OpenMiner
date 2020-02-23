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
#ifndef LUAGUI_HPP_
#define LUAGUI_HPP_

#include <list>

#include "LuaWidgetDef.hpp"
#include "ServerInfo.hpp"

struct LuaGUIData {
	std::list<LuaWidgetDef::Image> imageList;
	std::list<LuaWidgetDef::TextButton> textButtonList;
	std::list<LuaWidgetDef::InventoryWidget> inventoryWidgetList;
	std::list<LuaWidgetDef::CraftingWidget> craftingWidgetList;
	std::list<LuaWidgetDef::ProgressBarWidget> progressBarWidgetList;
};

// This class is meant to be used ONLY in Lua
class LuaGUI {
	public:
		void addImage(const sol::table &table);
		void addTextButton(const sol::table &table);
		void addInventoryWidget(const sol::table &table);
		void addCraftingWidget(const sol::table &table);
		void addProgressBarWidget(const sol::table &table);

		void show(Client &client);

		static void initUsertype(sol::state &lua);

		LuaGUIData m_data;
};

#endif // LUAGUI_HPP_
