/*
 * =====================================================================================
 *
 *       Filename:  LuaCore.hpp
 *
 *    Description:
 *
 *        Created:  07/01/2019 04:35:13
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef LUACORE_HPP_
#define LUACORE_HPP_

#include <functional>

#include <sol.hpp>

class Player;
class World;

class LuaCore {
	public:
		World *world() { return m_world; }
		void setWorld(World &world) { m_world = &world; }

		Player *player() { return m_player; }
		void setPlayer(Player &player) { m_player = &player; }

		static void initUsertype(sol::state &lua);

	private:
		Player *m_player = nullptr;
		World *m_world = nullptr;
};

#endif // LUACORE_HPP_
