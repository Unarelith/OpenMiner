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
#include <glm/gtc/matrix_transform.hpp>

#include <gk/resource/ResourceHandler.hpp>

#include "EngineConfig.hpp"
#include "World.hpp"

bool World::isReloadRequested = false;

Chunk *World::getChunkAtBlockPos(int x, int y, int z) const {
	return getChunk(
		std::floor((float)x / CHUNK_WIDTH),
		std::floor((float)y / CHUNK_DEPTH),
		std::floor((float)z / CHUNK_HEIGHT)
	);
}

BlockData *World::getBlockData(int x, int y, int z) const {
	Chunk *chunk = getChunkAtBlockPos(x, y, z);
	if (chunk)
		return chunk->getBlockData(x & (CHUNK_WIDTH - 1), y & (CHUNK_DEPTH - 1), z & (CHUNK_HEIGHT - 1));

	return nullptr;
}

BlockData *World::addBlockData(int x, int y, int z, int inventoryWidth, int inventoryHeight) const {
	Chunk *chunk = getChunkAtBlockPos(x, y, z);
	if (chunk)
		return chunk->addBlockData(x & (CHUNK_WIDTH - 1), y & (CHUNK_DEPTH - 1), z & (CHUNK_HEIGHT - 1), inventoryWidth, inventoryHeight);

	return nullptr;
}

u16 World::getBlock(int x, int y, int z) const {
	Chunk *chunk = getChunkAtBlockPos(x, y, z);
	if (chunk)
		return chunk->getBlock(x & (CHUNK_WIDTH - 1), y & (CHUNK_DEPTH - 1), z & (CHUNK_HEIGHT - 1));

	return 0;
}

void World::setBlock(int x, int y, int z, u16 id) const {
	Chunk *chunk = getChunkAtBlockPos(x, y, z);
	if (chunk)
		chunk->setBlock(x & (CHUNK_WIDTH - 1), y & (CHUNK_DEPTH - 1), z & (CHUNK_HEIGHT - 1), id);
}

u16 World::getData(int x, int y, int z) const {
	Chunk *chunk = getChunkAtBlockPos(x, y, z);
	if (chunk)
		return chunk->getData(x & (CHUNK_WIDTH - 1), y & (CHUNK_DEPTH - 1), z & (CHUNK_HEIGHT - 1));

	return 0;
}

void World::setData(int x, int y, int z, u16 data) const {
	Chunk *chunk = getChunkAtBlockPos(x, y, z);
	if (chunk)
		chunk->setData(x & (CHUNK_WIDTH - 1), y & (CHUNK_DEPTH - 1), z & (CHUNK_HEIGHT - 1), data);
}
