/*
 * =====================================================================================
 *
 *       Filename:  LuaWidget.hpp
 *
 *    Description:
 *
 *        Created:  08/02/2020 02:32:09
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef LUAWIDGET_HPP_
#define LUAWIDGET_HPP_

#include <gk/core/IntTypes.hpp>

namespace LuaWidget {
	enum : u8 {
		Image           = 0,
		TextButton      = 1,
		InventoryWidget = 2,
		CraftingWidget  = 3,
		FurnaceWidget   = 4,
		PlayerInventoryWidget = 5,
	};
}

#endif // LUAWIDGET_HPP_
