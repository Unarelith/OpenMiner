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
#include <gk/core/GameClock.hpp>

#include "Dimension.hpp"
#include "EngineConfig.hpp"
#include "Network.hpp"
#include "PlayerList.hpp"
#include "Server.hpp"
#include "ServerCommandHandler.hpp"
#include "ServerConfig.hpp"
#include "ServerPlayer.hpp"
#include "ServerWorld.hpp"

ServerWorld::ServerWorld(PlayerList &players, const Dimension &dimension, gk::GameClock &clock, s32 seed)
	: m_players(players),
	  m_dimension(dimension),
	  m_heightmap(seed),
	  m_terrainGenerator(m_heightmap, dimension, seed),
	  m_clock(clock),
	  m_scene(players),
	  m_seed(seed)
{
}

void ServerWorld::update(bool doTick) {
	World::update();

	while (!m_chunksToSend.empty()) {
		auto &[chunkPos, player] = m_chunksToSend.front();
		if (player.sentChunks.find(chunkPos) == player.sentChunks.end()) {
			glm::dvec3 chunkWorldPos{
				chunkPos.x * CHUNK_WIDTH + CHUNK_WIDTH / 2.f,
				chunkPos.y * CHUNK_DEPTH + CHUNK_DEPTH / 2.f,
				chunkPos.z * CHUNK_HEIGHT + CHUNK_HEIGHT / 2.f
			};

			glm::dvec3 playerPos{
				player.x(),
				player.y(),
				player.z()
			};

			if (glm::length(playerPos - chunkWorldPos) < (ServerConfig::renderDistance + 1) * CHUNK_WIDTH) {
				if (player.isOnline() && !player.isChunkLoaded(chunkPos)) {
					ServerChunk &chunk = getOrCreateChunk(chunkPos.x, chunkPos.y, chunkPos.z);
					generateChunk(chunk);
					player.addLoadedChunk(chunkPos);
					addChunkToUpdate(&chunk);
				}

				// gkDebug() << "OK for chunk" << chunkPos.x << chunkPos.y << chunkPos.z << ":" << glm::length(playerPos - chunkWorldPos) << "<" << (int)ServerConfig::renderDistance * CHUNK_WIDTH;

				auto addChunkToSend = [this](gk::Vector3i pos, s8 dx, s8 dy, s8 dz, ServerPlayer &player) {
					pos.x += dx; pos.y += dy; pos.z += dz;
					m_chunksToSend.emplace(std::make_pair(pos, std::ref(player)));
				};

				addChunkToSend(chunkPos,  1,  0,  0, player);
				addChunkToSend(chunkPos, -1,  0,  0, player);
				addChunkToSend(chunkPos,  0,  1,  0, player);
				addChunkToSend(chunkPos,  0, -1,  0, player);
				addChunkToSend(chunkPos,  0,  0,  1, player);
				addChunkToSend(chunkPos,  0,  0, -1, player);
			}

			player.sentChunks.emplace(chunkPos);
		}

		m_chunksToSend.pop();
	}

	if (doTick) {
		for (auto &it : m_chunks)
			it.second->tick();

		m_scene.update();
	}
}

void ServerWorld::updatePlayerChunks(ServerPlayer &player, s32 cx, s32 cy, s32 cz) {
	gk::Vector3i currentChunk{cx, cy, cz};
	player.sentChunks.clear();
	m_chunksToSend.emplace(std::make_pair(currentChunk, std::ref(player)));

	std::list<gk::Vector3i> chunksToRemove;
	for (auto &chunkPos : player.loadedChunks()) {
		glm::dvec3 chunkWorldPos{
			chunkPos.x * CHUNK_WIDTH + CHUNK_WIDTH / 2.f,
			chunkPos.y * CHUNK_DEPTH + CHUNK_DEPTH / 2.f,
			chunkPos.z * CHUNK_HEIGHT + CHUNK_HEIGHT / 2.f
		};

		glm::dvec3 playerPos{
			player.x(),
			player.y(),
			player.z()
		};

		if (glm::length(playerPos - chunkWorldPos) >= (ServerConfig::renderDistance + 1) * CHUNK_WIDTH) {
			m_server->sendChunkUnload(chunkPos.x, chunkPos.y, chunkPos.z, player.client());
			chunksToRemove.emplace_back(chunkPos);

			// gkDebug() << "Chunk at" << chunkPos.x << chunkPos.y << chunkPos.z << "unloaded for player" << player.name();
		}
	}

	for (auto &chunkPos : chunksToRemove)
		player.removeLoadedChunk(chunkPos);
}

void ServerWorld::createChunkNeighbours(ServerChunk &chunk) {
	gk::Vector3i surroundingChunks[6] = {
		{chunk.x() - 1, chunk.y(),     chunk.z()},
		{chunk.x() + 1, chunk.y(),     chunk.z()},
		{chunk.x(),     chunk.y() - 1, chunk.z()},
		{chunk.x(),     chunk.y() + 1, chunk.z()},
		{chunk.x(),     chunk.y(),     chunk.z() - 1},
		{chunk.x(),     chunk.y(),     chunk.z() + 1},
	};

	for (u8 i = 0 ; i < 6 ; ++i) {
		// Check if this neighbour already exists, if yes then skip it
		ServerChunk *neighbour = (ServerChunk *)getChunk(surroundingChunks[i].x, surroundingChunks[i].y, surroundingChunks[i].z);
		if (neighbour) {
			// Assign surrounding chunk pointers
			chunk.setSurroundingChunk(i, neighbour);
			neighbour->setSurroundingChunk((i % 2 == 0) ? i + 1 : i - 1, &chunk);

			continue;
		}

		// Create our neighbour
		auto it = m_chunks.emplace(
			gk::Vector3i{
				surroundingChunks[i].x,
				surroundingChunks[i].y,
				surroundingChunks[i].z
			},
			new ServerChunk{
				surroundingChunks[i].x,
				surroundingChunks[i].y,
				surroundingChunks[i].z,
				*this
			}
		);

		// Get the created neighbour
		neighbour = it.first->second.get();

		// Assign surrounding chunk pointers
		chunk.setSurroundingChunk(i, neighbour);
		neighbour->setSurroundingChunk((i % 2 == 0) ? i + 1 : i - 1, &chunk);
	}
}

ServerChunk &ServerWorld::getOrCreateChunk(s32 cx, s32 cy, s32 cz) {
	ServerChunk *chunk = (ServerChunk *)getChunk(cx, cy, cz);
	if (!chunk) {
		auto it = m_chunks.emplace(gk::Vector3i{cx, cy, cz}, new ServerChunk(cx, cy, cz, *this));
		chunk = it.first->second.get();
	}

	// Create our neighbours
	createChunkNeighbours(*chunk);

	return *chunk;
}

Chunk *ServerWorld::getChunk(int cx, int cy, int cz) const {
	auto it = m_chunks.find({cx, cy, cz});
	if (it == m_chunks.end())
		return nullptr;

	return it->second.get();
}

void ServerWorld::generateChunk(ServerChunk &chunk) {
	if (!chunk.isInitialized()) {
		m_terrainGenerator.generate(chunk);

		chunk.setInitialized(true);
	}
}

// Please update 'docs/lua-api-cpp.md' if you change this
void ServerWorld::initUsertype(sol::state &lua) {
	lua.new_usertype<ServerWorld>("ServerWorld",
		sol::base_classes, sol::bases<World>(),

		"dimension", &ServerWorld::dimension
	);
}

