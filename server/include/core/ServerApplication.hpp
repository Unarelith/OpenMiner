/*
 * =====================================================================================
 *
 *       Filename:  ServerApplication.hpp
 *
 *    Description:
 *
 *        Created:  09/01/2019 19:37:20
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef SERVERAPPLICATION_HPP_
#define SERVERAPPLICATION_HPP_

#include <gk/core/CoreApplication.hpp>

#include "LuaCore.hpp"
#include "Registry.hpp"
#include "ScriptEngine.hpp"
#include "Server.hpp"
#include "ServerCommandHandler.hpp"
#include "ServerPlayer.hpp"
#include "ServerWorld.hpp"

class ServerApplication : public gk::CoreApplication {
	public:
		ServerApplication(int argc, char **argv);

		void init() override;

	private:
		void mainLoop() override;

		Server m_server;
		ServerWorld m_world;
		std::vector<ServerPlayer> m_players;

		ScriptEngine m_scriptEngine;
		LuaCore m_luaCore;

		Registry m_registry;

		int m_port = 4242;

		ServerCommandHandler m_serverCommandHandler{m_scriptEngine, m_server, m_world, m_players, m_registry};
};

#endif // SERVERAPPLICATION_HPP_
