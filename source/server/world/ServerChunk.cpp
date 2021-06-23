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
#include "ClientInfo.hpp"
#include "Dimension.hpp"
#include "Network.hpp"
#include "Player.hpp"
#include "PlayerList.hpp"
#include "Server.hpp"
#include "ServerBlock.hpp"
#include "ServerChunk.hpp"
#include "ServerCommandHandler.hpp"
#include "ServerWorld.hpp"
#include "World.hpp"

ServerChunk::ServerChunk(s32 x, s32 y, s32 z, ServerWorld &world) : Chunk(x, y, z, world), m_world(world) {
	m_random.seed((unsigned int)std::time(nullptr));
}

void ServerChunk::update() {
	if (areAllNeighboursInitialized())
		m_lightmap.updateLights();

	if (m_hasChanged || m_lightmap.hasChanged()) {
		// gkDebug() << "Chunk update at" << m_x << m_y << m_z << "| D:" << m_hasChanged << "| L:" << m_lightmap.hasChanged();

		m_hasChanged = false;
		m_lightmap.resetChangedFlag();

		if (m_isInitialized && m_isReadyToSend)
			m_world.addChunkToProcess(this);
	}
}

void ServerChunk::process() {
	for (auto &client : m_world.server()->server().info().clients()) {
		const ServerPlayer *player = m_world.players().getPlayer(client.playerName);
		bool isChunkLoaded = player->isChunkLoaded({m_x, m_y, m_z});
		if (player->isReady() && isChunkLoaded && player->dimension() == m_world.dimension().id())
			sendData(client);
	}
}

void ServerChunk::onBlockPlaced(int x, int y, int z, const Block &block) {
	const ServerBlock &serverBlock = (ServerBlock &)block;
	if (block.canUpdate()) {
		addTickingBlock(x, y, z, serverBlock);
	}
	else {
		auto it = m_tickingBlocks.find(gk::Vector3i{x, y, z});
		if (it != m_tickingBlocks.end())
			m_tickingBlocks.erase(it);
	}

	serverBlock.onBlockPlaced(glm::ivec3{x + m_x * CHUNK_WIDTH, y + m_y * CHUNK_DEPTH, z + m_z * CHUNK_HEIGHT}, (ServerWorld &)m_world);

	m_hasBeenModified = true;
}

void ServerChunk::onBlockDestroyed(int x, int y, int z, const Block &block) {
	const ServerBlock &serverBlock = (ServerBlock &)block;
	serverBlock.onBlockDestroyed(glm::ivec3{x + m_x * CHUNK_WIDTH, y + m_y * CHUNK_DEPTH, z + m_z * CHUNK_HEIGHT}, (ServerWorld &)m_world);

	m_hasBeenModified = true;
}

void ServerChunk::tick() {
	if (!m_tickingBlocks.empty()) {
		for (auto &it : m_tickingBlocks) {
			if (!it.second.isTickingRandomly() || m_random.get<bool>(it.second.tickProbability())) {
				it.second.onTick(
					glm::ivec3{
						it.first.x + m_x * width,
						it.first.y + m_y * depth,
						it.first.z + m_z * height
					},
				*this, m_world, *m_world.server());
			}
		}
	}
}

void ServerChunk::sendData(const ClientInfo &client) {
	Network::Packet packet;
	packet << Network::Command::ChunkData;
	packet << m_x << m_y << m_z;
	for (u16 z = 0 ; z < CHUNK_HEIGHT ; ++z) {
		for (u16 y = 0 ; y < CHUNK_DEPTH ; ++y) {
			for (u16 x = 0 ; x < CHUNK_WIDTH ; ++x) {
				packet << data(x, y, z);
				packet << m_lightmap.getLightDataRaw(x, y, z);

				BlockData *blockData = getBlockData(x, y, z);
				if (blockData) {
					s32 globalX = x + m_x * CHUNK_WIDTH;
					s32 globalY = y + m_y * CHUNK_DEPTH;
					s32 globalZ = z + m_z * CHUNK_HEIGHT;

					m_world.server()->sendBlockDataUpdate(globalX, globalY, globalZ, blockData, &client);
					m_world.server()->sendBlockInvUpdate(globalX, globalY, globalZ, blockData->inventory, &client);
				}
			}
		}
	}

	client.tcpSocket->send(packet);

	// gkDebug() << "Chunk at" << chunk.x() << chunk.y() << chunk.z() << "sent to client";
}

