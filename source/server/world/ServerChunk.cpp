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
#include "Player.hpp"
#include "ServerBlock.hpp"
#include "ServerChunk.hpp"
#include "ServerCommandHandler.hpp"
#include "World.hpp"

ServerChunk::ServerChunk(s32 x, s32 y, s32 z, World &world) : Chunk(x, y, z, world) {
	m_random.seed(std::time(nullptr));
}

void ServerChunk::updateLights() {
	if (m_lightmap.updateLights() || m_hasChanged) {
		m_isSent = false;
		m_hasChanged = false;
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

void ServerChunk::tick(World &world, ServerCommandHandler &server) {
	if (!m_tickingBlocks.empty()) {
		for (auto &it : m_tickingBlocks) {
			if (!it.second.isTickingRandomly() || m_random.get<bool>(it.second.tickProbability())) {
				it.second.onTick(
					glm::ivec3{
						it.first.x + m_x * width,
						it.first.y + m_y * depth,
						it.first.z + m_z * height
					},
				*this, (ServerWorld &)world, server);
			}
		}
	}
}

