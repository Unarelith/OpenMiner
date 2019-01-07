/*
 * =====================================================================================
 *
 *       Filename:  LuaGUI.hpp
 *
 *    Description:
 *
 *        Created:  06/01/2019 23:20:23
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef LUAGUI_HPP_
#define LUAGUI_HPP_

#include "LuaWidgetDef.hpp"

// This class is meant to be used ONLY in Lua
class LuaGUI {
	public:
		void addImage(const sol::table &table);
		void addButton(const sol::table &table);
		void addInventory(const sol::table &table);

		void show();

		std::vector<LuaWidgetDef::Image> images;
		std::vector<LuaWidgetDef::Button> buttons;
		std::vector<LuaWidgetDef::InventoryList> inventoryLists;
};

#endif // LUAGUI_HPP_
