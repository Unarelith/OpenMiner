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
#include <cstring>

#include "ServerChunk.hpp"

void ServerChunk::update() {
	// FIXME
	// if (!m_tickingBlocks.empty() && m_lastTick < gk::GameClock::getTicks() / 50) {
	// 	m_lastTick = gk::GameClock::getTicks() / 50;
    //
	// 	for (auto &it : m_tickingBlocks) {
	// 		int z = it.first / (width * height);
	// 		int y = (it.first - z * width * height) / width;
	// 		int x = (it.first - z * width * height) % width;
	// 		it.second.onTick(glm::ivec3{x + m_x * width, y + m_y * height, z + m_z * depth}, player, *this, world);
	// 	}
	// }

	if (m_hasChanged) {
		m_lightmap.updateLights();
		m_hasChanged = false;
		m_isSent = false;
	}
}

void ServerChunk::generate() {
	if (!m_isGenerated) {
		m_terrainGenerator.generate(*this);

		m_isGenerated = true;
		m_hasChanged = true;
	}
}

