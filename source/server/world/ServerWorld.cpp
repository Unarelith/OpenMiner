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
	{
		for (auto &it : m_players) {
			if (it.second.isReady() && it.second.dimension() == m_dimension.id()) {
				gk::Vector3i currentChunk = it.second.getCurrentChunk();
				if (!it.second.isChunkLoaded(currentChunk) || it.second.lastChunkUpdate != currentChunk) {
					m_chunksToSend.emplace(std::make_pair(currentChunk, std::ref(it.second)));
					it.second.addLoadedChunk(currentChunk);
					it.second.lastChunkUpdate = currentChunk;
				}
			}
		}

		auto addChunkToSend = [this](gk::Vector3i pos, s8 dx, s8 dy, s8 dz, ServerPlayer &player) {
			pos.x += dx; pos.y += dy; pos.z += dz;
			m_chunksToSend.emplace(std::make_pair(pos, std::ref(player)));
		};

		while (!m_chunksToSend.empty()) {
			auto &[chunkPos, player] = m_chunksToSend.front();
			if (!player.isChunkLoaded(chunkPos)) {
				sendRequestedData(*player.client(), chunkPos.x, chunkPos.y, chunkPos.z);
				player.addLoadedChunk(chunkPos);
			}

			gk::Vector3i playerChunkPos = player.getCurrentChunk();
			if ((playerChunkPos - chunkPos).length() <= ServerConfig::renderDistance) {
				addChunkToSend(chunkPos,  1,  0,  0, player);
				addChunkToSend(chunkPos, -1,  0,  0, player);
				addChunkToSend(chunkPos,  0,  1,  0, player);
				addChunkToSend(chunkPos,  0, -1,  0, player);
				addChunkToSend(chunkPos,  0,  0,  1, player);
				addChunkToSend(chunkPos,  0,  0, -1, player);
			}

			m_chunksToSend.pop();
		}
	}

	for (auto &it : m_chunks) {
		if (doTick)
			it.second->tick(*this, *m_server);

		if (it.second->areAllNeighboursLoaded()) {
			it.second->updateLights();
		}

		// if (it.second->isInitialized() && !it.second->isSent()) {
		// 	for (auto &client : m_server->server().info().clients()) {
		// 		ServerPlayer *player = m_players.getPlayer(client.playerName);
		// 		if (player->isReady() && player->dimension() == m_dimension.id())
		// 			sendChunkData(client, *it.second.get());
		// 	}
        //
		// 	// gkDebug() << "Chunk updated at" << it.second->x() << it.second->y() << it.second->z();
		// }
	}

	if (doTick)
		m_scene.update();
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

void ServerWorld::sendChunkData(const ClientInfo &client, ServerChunk &chunk) {
	Network::Packet packet;
	packet << Network::Command::ChunkData;
	packet << chunk.x() << chunk.y() << chunk.z();
	for (u16 z = 0 ; z < CHUNK_HEIGHT ; ++z) {
		for (u16 y = 0 ; y < CHUNK_DEPTH ; ++y) {
			for (u16 x = 0 ; x < CHUNK_WIDTH ; ++x) {
				packet << chunk.data(x, y, z);
				packet << chunk.lightmap().getLightData(x, y, z);

				BlockData *blockData = chunk.getBlockData(x, y, z);
				if (blockData) {
					s32 globalX = x + chunk.x() * CHUNK_WIDTH;
					s32 globalY = y + chunk.y() * CHUNK_DEPTH;
					s32 globalZ = z + chunk.z() * CHUNK_HEIGHT;

					m_server->sendBlockDataUpdate(globalX, globalY, globalZ, blockData, &client);
					m_server->sendBlockInvUpdate(globalX, globalY, globalZ, blockData->inventory, &client);
				}
			}
		}
	}

	client.tcpSocket->send(packet);
	chunk.setSent(true);
	chunk.setChanged(false);

	gkDebug() << "Chunk at" << chunk.x() << chunk.y() << chunk.z() << "sent to client";
}

void ServerWorld::sendRequestedData(const ClientInfo &client, int cx, int cy, int cz) {
	ServerChunk &chunk = getOrCreateChunk(cx, cy, cz);

	generateChunk(chunk);

	sendChunkData(client, chunk);
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

	chunk.updateLights();
}

// Please update 'docs/lua-api-cpp.md' if you change this
void ServerWorld::initUsertype(sol::state &lua) {
	lua.new_usertype<ServerWorld>("ServerWorld",
		sol::base_classes, sol::bases<World>(),

		"dimension", &ServerWorld::dimension
	);
}

