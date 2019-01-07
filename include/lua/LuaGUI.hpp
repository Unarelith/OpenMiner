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

#include <sol.hpp>

#include <gk/core/IntTypes.hpp>

// FIXME: Move this namespace to its own file
namespace GUIDefinition {

struct Widget {
	std::string name;

	float x = 0;
	float y = 0;
};

struct Button : public Widget {
	std::string text;
	sol::function on_click;
};

struct InventoryList : public Widget {
	std::string player;
	std::string inventory;

	float width = 0;
	float height = 0;

	u16 offset = 0;
};

} // namespace GUIDefinition

// This class is meant to be used ONLY in Lua
class LuaGUI {
	public:
		void addButton(const sol::table &table);
		void addInventory(const sol::table &table);

		void show();

		std::vector<GUIDefinition::Button> buttons;
};

#endif // LUAGUI_HPP_
