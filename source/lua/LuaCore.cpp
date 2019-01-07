/*
 * =====================================================================================
 *
 *       Filename:  LuaCore.cpp
 *
 *    Description:
 *
 *        Created:  07/01/2019 04:41:11
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "LuaCore.hpp"
#include "Player.hpp"
#include "World.hpp"

void LuaCore::initUsertype(sol::state &lua) {
	lua.new_usertype<LuaCore>("LuaCore",
		"world",  &LuaCore::world,
		"player", &LuaCore::player
	);
}

