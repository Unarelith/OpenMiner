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

class Registry;
class ServerPlayer;
class ServerWorld;

class LuaCore {
	public:
		ServerWorld *world() { return m_world; }
		void setWorld(ServerWorld &world) { m_world = &world; }

		ServerPlayer *player() { return m_player; }
		void setPlayer(ServerPlayer &player) { m_player = &player; }

		Registry *registry();

		static void initUsertype(sol::state &lua);

	private:
		ServerPlayer *m_player = nullptr;
		ServerWorld *m_world = nullptr;
};

#endif // LUACORE_HPP_
