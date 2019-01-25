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

#include <memory>
#include <vector>

#include "ServerChunk.hpp"
#include "World.hpp"

struct Client;
class Server;

class ServerWorld : public World {
	public:
		ServerWorld();

		void update(Server &server);

		void sendWorldData(Client &client);
		void sendChunkData(Client &client, ServerChunk *chunk);
		void sendRequestedData(Client &client, int cx, int cy, int cz);

		// FIXME: Duplicated with ClientWorld
		ServerChunk *getChunk(int cx, int cy, int cz) const;
		BlockData *getBlockData(int x, int y, int z) const override;

		// FIXME: Duplicated with ClientWorld
		u16 getBlock(int x, int y, int z) const override;
		void setBlock(int x, int y, int z, u16 id) override;

	private:
		std::vector<std::unique_ptr<ServerChunk>> m_chunks;
};

#endif // SERVERWORLD_HPP_
