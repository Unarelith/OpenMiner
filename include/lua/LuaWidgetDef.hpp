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
#include <gk/core/Vector3.hpp>

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

struct TextButton : public Widget {
	std::string text;
	sol::function on_click;
};

struct InventoryWidget : public Widget {
	std::string player;
	std::string inventory;

	float width = 0;
	float height = 0;

	u16 offset = 0;
	u16 count = 0;
};

struct CraftingWidget : public Widget {
	gk::Vector3i block;

	u16 offset = 0;
	u16 count = 9;
};

} // namespace LuaWidgetDef

#endif // LUAWIDGETDEF_HPP_
