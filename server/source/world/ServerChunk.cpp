/*
 * =====================================================================================
 *
 *       Filename:  ServerChunk.cpp
 *
 *    Description:
 *
 *        Created:  12/01/2019 01:33:16
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "Player.hpp"
#include "Server.hpp"
#include "ServerBlock.hpp"
#include "ServerChunk.hpp"
#include "World.hpp"

void ServerChunk::generate() {
	if (!m_isGenerated) {
		m_terrainGenerator.generate(*this);

		m_isGenerated = true;
	}
}

void ServerChunk::updateLights() {
	if (m_lightmap.updateLights()) {
		m_isSent = false;
	}
}

void ServerChunk::tick(std::unordered_map<u16, ServerPlayer> &players, World &world, Server &server) {
	if (!m_tickingBlocks.empty()) {
		for (auto &it : m_tickingBlocks) {
			int z = it.first / (width * height);
			int y = (it.first - z * width * height) / width;
			int x = (it.first - z * width * height) % width;
			((ServerBlock &)it.second).onTick(
				glm::ivec3{x + m_x * width, y + m_y * height, z + m_z * depth},
				players, *this, world, server);
		}
	}
}

