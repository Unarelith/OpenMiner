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

class Client;

class ServerWorld {
	public:
		ServerWorld();

		void update();

		void sendWorldData(Client &client);
		void sendChunkData(Client &client, ServerChunk *chunk);

		ServerChunk *getChunk(int cx, int cy, int cz) const;

	private:
		// FIXME: Duplicated with those in ClientWorld
		const s32 m_width = 8;
		const s32 m_height = 4;
		const s32 m_depth = 8;

		std::vector<std::unique_ptr<ServerChunk>> m_chunks;
};

#endif // SERVERWORLD_HPP_
