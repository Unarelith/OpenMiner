/*
 * =====================================================================================
 *
 *  OpenMiner
 *
 *  Copyright (C) 2018-2020 Unarelith, Quentin Bazin <openminer@unarelith.net>
 *  Copyright (C) 2019-2020 the OpenMiner contributors (see CONTRIBUTORS.md)
 *
 *  This file is part of OpenMiner.
 *
 *  OpenMiner is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  OpenMiner is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with OpenMiner; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *
 * =====================================================================================
 */
#ifndef SERVERWORLD_HPP_
#define SERVERWORLD_HPP_

#include <unordered_map>

#include "ServerChunk.hpp"
#include "ServerScene.hpp"
#include "TerrainGenerator.hpp"
#include "World.hpp"

namespace gk {
	class GameClock;
}

class ClientInfo;
class Dimension;
class PlayerList;
class ServerCommandHandler;
class ServerPlayer;

class ServerWorld : public World {
	using ChunkMap = std::unordered_map<gk::Vector3i, std::unique_ptr<ServerChunk>>;

	public:
		ServerWorld(PlayerList &players, const Dimension &dimension, gk::GameClock &clock, s32 seed);

		void update(bool doTick);

		void createChunkNeighbours(ServerChunk &chunk);
		void sendChunkData(const ClientInfo &client, ServerChunk &chunk);
		void sendRequestedData(ClientInfo &client, s32 cx, s32 cy, s32 cz);

		ServerChunk &getOrCreateChunk(s32 cx, s32 cy, s32 cz);

		Chunk *getChunk(int cx, int cy, int cz) const override;

		void generateChunk(ServerChunk &chunk);

		const Dimension &dimension() const { return m_dimension; }

		const ChunkMap &chunks() const { return m_chunks; }

		Heightmap &heightmap() { return m_heightmap; }

		TerrainGenerator &terrainGenerator() { return m_terrainGenerator; }

		ServerScene &scene() { return m_scene; }

		void setServer(ServerCommandHandler *server) { m_server = server; m_scene.setServer(server); }

		s32 seed() const { return m_seed; }

		static void initUsertype(sol::state &lua);

	private:
		PlayerList &m_players;

		const Dimension &m_dimension;

		ChunkMap m_chunks;

		Heightmap m_heightmap;
		TerrainGenerator m_terrainGenerator;

		ServerCommandHandler *m_server = nullptr;

		gk::GameClock &m_clock;

		ServerScene m_scene;

		s32 m_seed = 0;
};

#endif // SERVERWORLD_HPP_
