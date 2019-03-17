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

#include <gk/core/Vector3.hpp>

class Registry;
class Server;
class ServerPlayer;
class ServerWorld;

class ServerCommandHandler {
	public:
		ServerCommandHandler(Server &server, ServerWorld &world, ServerPlayer &player, Registry &registry)
			: m_server(server), m_world(world), m_player(player), m_registry(registry) {}

		void setupCallbacks();

	private:
		Server &m_server;
		ServerWorld &m_world;
		ServerPlayer &m_player;

		Registry &m_registry;

		gk::Vector3<s32> m_spawnPosition{0, 22, 20};
};

#endif // SERVERCOMMANDHANDLER_HPP_
