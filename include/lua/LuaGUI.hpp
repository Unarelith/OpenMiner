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

#include <list>

#include "LuaWidgetDef.hpp"

// This class is meant to be used ONLY in Lua
class LuaGUI {
	public:
		void addImage(const sol::table &table);
		void addTextButton(const sol::table &table);
		void addInventoryWidget(const sol::table &table);
		void addCraftingWidget(const sol::table &table);

		void show();

		static void initUsertype(sol::state &lua);

		std::list<LuaWidgetDef::Image> imageList;
		std::list<LuaWidgetDef::TextButton> textButtonList;
		std::list<LuaWidgetDef::InventoryWidget> inventoryWidgetList;
		std::list<LuaWidgetDef::CraftingWidget> craftingWidgetList;
};

#endif // LUAGUI_HPP_
