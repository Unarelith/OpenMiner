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
#include "ServerInfo.hpp"

struct LuaGUIData {
	enum {
		Image,
		TextButton,
		InventoryWidget,
		CraftingWidget,
		FurnaceWidget,
	};

	std::list<LuaWidgetDef::Image> imageList;
	std::list<LuaWidgetDef::TextButton> textButtonList;
	std::list<LuaWidgetDef::InventoryWidget> inventoryWidgetList;
	std::list<LuaWidgetDef::CraftingWidget> craftingWidgetList;
	std::list<LuaWidgetDef::FurnaceWidget> furnaceWidgetList;
};

// This class is meant to be used ONLY in Lua
class LuaGUI {
	public:
		void addImage(const sol::table &table);
		void addTextButton(const sol::table &table);
		void addInventoryWidget(const sol::table &table);
		void addCraftingWidget(const sol::table &table);
		void addFurnaceWidget(const sol::table &table);

		void show(Client &client);

		static void initUsertype(sol::state &lua);

		LuaGUIData m_data;
};

#endif // LUAGUI_HPP_
