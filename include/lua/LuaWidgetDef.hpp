/*
 * =====================================================================================
 *
 *       Filename:  LuaWidgetDef.hpp
 *
 *    Description:
 *
 *        Created:  07/01/2019 01:29:24
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef LUAWIDGETDEF_HPP_
#define LUAWIDGETDEF_HPP_

#include <sol.hpp>

#include <gk/core/IntTypes.hpp>
#include <gk/core/Rect.hpp>

namespace LuaWidgetDef {

struct Widget {
	std::string name;

	float x = 0;
	float y = 0;
};

struct Image : public Widget {
	std::string texture;
	gk::FloatRect clipRect;
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
	u16 size = 0;
};

} // namespace LuaWidgetDef

#endif // LUAWIDGETDEF_HPP_
