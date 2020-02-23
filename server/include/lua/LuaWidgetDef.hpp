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
#ifndef LUAWIDGETDEF_HPP_
#define LUAWIDGETDEF_HPP_

#include <sol.hpp>

#include <gk/core/IntTypes.hpp>
#include <gk/core/Rect.hpp>
#include <gk/core/Vector3.hpp>

namespace LuaWidgetDef {

struct Widget {
	std::string name;

	s32 x = 0;
	s32 y = 0;
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
	u16 size = 3;

	s32 resultX = 0;
	s32 resultY = 0;
};

struct FurnaceWidget : public Widget {
	gk::Vector3i block;
};

struct PlayerCraftingWidget : public Widget {
	s32 resultX = 0;
	s32 resultY = 0;
};

} // namespace LuaWidgetDef

#endif // LUAWIDGETDEF_HPP_
