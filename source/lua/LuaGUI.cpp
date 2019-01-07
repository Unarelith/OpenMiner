/*
 * =====================================================================================
 *
 *       Filename:  LuaGUI.cpp
 *
 *    Description:
 *
 *        Created:  06/01/2019 23:20:36
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include <gk/core/ApplicationStateStack.hpp>

#include "LuaGUI.hpp"
#include "LuaGUIState.hpp"

void LuaGUI::addButton(const sol::table &table) {
	// FIXME: Duplicated below
	float x = 0, y = 0;
	sol::optional<sol::table> pos = table["pos"];
	std::string name = table["name"].get<std::string>();
	if (pos != sol::nullopt) {
		x = pos.value()["x"];
		y = pos.value()["y"];
	}

	std::string text = table["text"].get<std::string>();
	sol::function on_click = table["on_click"].get<sol::function>();
	buttons.emplace_back(GUIDefinition::Button{{name, x, y}, text, on_click});
}

void LuaGUI::addInventory(const sol::table &table) {
	// // FIXME: Duplicated above
	// float x = 0, y = 0;
	// sol::optional<sol::table> pos = table["pos"];
	// std::string name = table["name"].get<std::string>();
	// if (pos != sol::nullopt) {
	// 	x = pos.value()["x"];
	// 	y = pos.value()["y"];
	// }
    //
	// std::string player = table["player"].get<std::string>();
	// std::string inventory = table["inventory"].get<std::string>();
	// u16 offset = table["offset"].get<u16>();
    //
	// float width = 0, height = 0;
	// sol::optional<sol::table> size = table["size"];
	// if (size != sol::nullopt) {
	// 	width = size.value()["x"];
	// 	height = size.value()["y"];
	// }
    //
	// inventoryLists.emplace_back(name, x, y, player, inventory, width, height, offset);
}

void LuaGUI::show() {
	auto &stateStack = gk::ApplicationStateStack::getInstance();
	stateStack.push<LuaGUIState>(*this, &stateStack.top());
}

