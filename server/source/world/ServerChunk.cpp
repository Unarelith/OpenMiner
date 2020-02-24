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

void ServerChunk::updateLights() {
	if (m_lightmap.updateLights() || m_hasChanged) {
		m_isSent = false;
		m_hasChanged = false;
	}
}

void ServerChunk::onBlockPlaced(int x, int y, int z, const Block &block) const {
	const ServerBlock &serverBlock = (ServerBlock &)block;
	serverBlock.onBlockPlaced(glm::ivec3{x + m_x * CHUNK_WIDTH, y + m_y * CHUNK_DEPTH, z + m_z * CHUNK_HEIGHT}, m_world);
}

void ServerChunk::tick(std::unordered_map<u16, ServerPlayer> &players, World &world, ServerCommandHandler &server) {
	if (!m_tickingBlocks.empty()) {
		for (auto &it : m_tickingBlocks) {
			int z = it.first / (width * height);
			int y = (it.first - z * (width * height)) / width;
			int x = (it.first - z * (width * height)) % width;
			((ServerBlock &)it.second).onTick(
				glm::ivec3{x + m_x * width, y + m_y * depth, z + m_z * height},
				players, *this, world, server);
		}
	}
}

