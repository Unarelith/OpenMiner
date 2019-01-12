/*
 * =====================================================================================
 *
 *       Filename:  World.cpp
 *
 *    Description:
 *
 *        Created:  16/12/2014 15:28:19
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#include <gk/resource/ResourceHandler.hpp>

#include "Config.hpp"
#include "World.hpp"

u16 World::renderDistance = 8;
bool World::isReloadRequested = false;

void World::update() {
	// FIXME
	// for (auto &it : m_chunks) {
	// 	if (isReloadRequested)
	// 		it->setChanged(true);
    //
	// 	it->update(player, *this);
	// }

	isReloadRequested = false;
}

BlockData *World::getBlockData(int x, int y, int z) {
	int cx = (x + CHUNK_WIDTH * (m_width / 2)) / CHUNK_WIDTH;
	int cy = (y + CHUNK_HEIGHT * (m_height / 2)) / CHUNK_HEIGHT;
	int cz = (z + CHUNK_DEPTH * (m_depth / 2)) / CHUNK_DEPTH;

	if (cx < 0 || cx >= m_width || cy < 0 || cy >= m_height || cz < 0 || cz >= m_depth)
		return 0;

	// FIXME
	// Chunk *chunk = m_chunks.at(cx + cy * m_width + cz * m_width * m_height).get();
	// if (chunk)
	// 	return chunk->getBlockData(x & (CHUNK_WIDTH - 1), y & (CHUNK_HEIGHT - 1), z & (CHUNK_DEPTH - 1));
	return 0;
}

