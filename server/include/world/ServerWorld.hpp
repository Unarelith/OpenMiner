/*
 * =====================================================================================
 *
 *  OpenMiner
 *  Copyright (C) 2018-2020 Unarelith, Quentin Bazin <openminer@unarelith.net>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
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
class ServerCommandHandler;
class ServerPlayer;

class ServerWorld : public World {
	public:
		ServerWorld() = default;

		void update(std::unordered_map<u16, ServerPlayer> &players);

		void createChunkNeighbours(ServerChunk *chunk);
		void sendChunkData(const Client &client, ServerChunk *chunk);
		void sendRequestedData(Client &client, s32 cx, s32 cy, s32 cz);

		Chunk *getChunk(int cx, int cy, int cz) const override;

		TerrainGenerator &terrainGenerator() { return m_terrainGenerator; }

		void setServer(ServerCommandHandler *server) { m_server = server; }

	private:
		std::unordered_map<gk::Vector3i, std::unique_ptr<ServerChunk>> m_chunks;
		std::unordered_map<u16, std::queue<ServerChunk *>> m_chunkQueues;

		u32 m_lastTick = 0;

		TerrainGenerator m_terrainGenerator;

		ServerCommandHandler *m_server = nullptr;
};

#endif // SERVERWORLD_HPP_
