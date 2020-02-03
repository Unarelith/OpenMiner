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
#include "TerrainGenerator.hpp"
#include "World.hpp"

class Client;
class Server;
class ServerPlayer;

class ServerWorld : public World {
	public:
		ServerWorld();

		void update(Server &server, std::unordered_map<u16, ServerPlayer> &players);

		void createChunkNeighbours(ServerChunk *chunk);
		void sendChunkData(const Client &client, ServerChunk *chunk);
		void sendRequestedData(Client &client, s32 cx, s32 cy, s32 cz);

		Chunk *getChunk(int cx, int cy, int cz) const override;

	private:
		std::unordered_map<gk::Vector3i, std::unique_ptr<ServerChunk>> m_chunks;
		std::unordered_map<u16, std::queue<ServerChunk *>> m_chunkQueues;

		u32 m_lastTick = 0;

		TerrainGenerator m_terrainGenerator;
};

#endif // SERVERWORLD_HPP_
