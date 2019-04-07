/*
 * =====================================================================================
 *
 *       Filename:  ServerCommandHandler.hpp
 *
 *    Description:
 *
 *        Created:  17/03/2019 17:12:17
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef SERVERCOMMANDHANDLER_HPP_
#define SERVERCOMMANDHANDLER_HPP_

#include <vector>

#include <gk/core/Vector3.hpp>

class Registry;
class ScriptEngine;
class Server;
class ServerPlayer;
class ServerWorld;

class ServerCommandHandler {
	public:
		ServerCommandHandler(ScriptEngine &scriptEngine, Server &server, ServerWorld &world, std::vector<ServerPlayer> &players, Registry &registry)
			: m_scriptEngine(scriptEngine), m_server(server), m_world(world), m_players(players), m_registry(registry) {}

		void setupCallbacks();

	private:
		ScriptEngine &m_scriptEngine;

		Server &m_server;
		ServerWorld &m_world;
		std::vector<ServerPlayer> &m_players;

		Registry &m_registry;

		gk::Vector3<s32> m_spawnPosition{0, 22, 20};
};

#endif // SERVERCOMMANDHANDLER_HPP_
