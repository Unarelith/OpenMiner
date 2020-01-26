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

#include <unordered_map>

#include <gk/core/Vector3.hpp>

class Registry;
class ScriptEngine;
class Server;
class ServerPlayer;
class ServerWorld;

class ServerCommandHandler {
	public:
		ServerCommandHandler(ScriptEngine &scriptEngine, Server &server, ServerWorld &world, std::unordered_map<u16, ServerPlayer> &players, Registry &registry)
			: m_scriptEngine(scriptEngine), m_server(server), m_world(world), m_players(players), m_registry(registry) {}

		void setupCallbacks();

	private:
		ScriptEngine &m_scriptEngine;

		Server &m_server;
		ServerWorld &m_world;
		std::unordered_map<u16, ServerPlayer> &m_players;

		Registry &m_registry;

		// FIXME: Warning: Duplicated in ClientPlayer.cpp
		gk::Vector3<s32> m_spawnPosition{12, 18, 12};
};

#endif // SERVERCOMMANDHANDLER_HPP_
