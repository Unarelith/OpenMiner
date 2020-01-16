/*
 * =====================================================================================
 *
 *       Filename:  ServerWorld.hpp
 *
 *    Description:
 *
 *        Created:  12/01/2019 14:02:07
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef SERVERWORLD_HPP_
#define SERVERWORLD_HPP_

#include <unordered_map>

#include "ServerChunk.hpp"
#include "World.hpp"

class Client;
class Server;
class ServerPlayer;

class ServerWorld : public World {
	public:
		ServerWorld();

		void update(Server &server, std::unordered_map<u16, ServerPlayer> &players);

		void sendSpawnData(Client &client, ServerPlayer &player);
		void sendChunkData(Client &client, ServerChunk *chunk);

		Chunk *getChunk(int cx, int cy, int cz) const override;

	private:
		std::unordered_map<gk::Vector3i, std::unique_ptr<ServerChunk>> m_chunks;

		u32 m_lastTick = 0;
};

#endif // SERVERWORLD_HPP_
