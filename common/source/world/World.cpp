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
#include <glm/gtc/matrix_transform.hpp>

#include <gk/resource/ResourceHandler.hpp>

#include "Config.hpp"
#include "World.hpp"

bool World::isReloadRequested = false;

BlockData *World::getBlockData(int x, int y, int z) const {
	Chunk *chunk = getChunk(
		std::floor((float)x / CHUNK_WIDTH),
		std::floor((float)y / CHUNK_HEIGHT),
		std::floor((float)z / CHUNK_DEPTH)
	);

	if (chunk)
		return chunk->getBlockData(x & (CHUNK_WIDTH - 1), y & (CHUNK_HEIGHT - 1), z & (CHUNK_DEPTH - 1));

	return nullptr;
}

u16 World::getBlock(int x, int y, int z) const {
	Chunk *chunk = getChunk(
		std::floor((float)x / CHUNK_WIDTH),
		std::floor((float)y / CHUNK_HEIGHT),
		std::floor((float)z / CHUNK_DEPTH)
	);

	if (chunk)
		return chunk->getBlock(x & (CHUNK_WIDTH - 1), y & (CHUNK_HEIGHT - 1), z & (CHUNK_DEPTH - 1));

	return 0;
}

void World::setBlock(int x, int y, int z, u16 id) const {
	Chunk *chunk = getChunk(
		std::floor((float)x / CHUNK_WIDTH),
		std::floor((float)y / CHUNK_HEIGHT),
		std::floor((float)z / CHUNK_DEPTH)
	);

	if (chunk)
		chunk->setBlock(x & (CHUNK_WIDTH - 1), y & (CHUNK_HEIGHT - 1), z & (CHUNK_DEPTH - 1), id);
}

u16 World::getData(int x, int y, int z) const {
	Chunk *chunk = getChunk(
		std::floor((float)x / CHUNK_WIDTH),
		std::floor((float)y / CHUNK_HEIGHT),
		std::floor((float)z / CHUNK_DEPTH)
	);

	if (chunk)
		return chunk->getData(x & (CHUNK_WIDTH - 1), y & (CHUNK_HEIGHT - 1), z & (CHUNK_DEPTH - 1));

	return 0;
}

void World::setData(int x, int y, int z, u16 id) const {
	Chunk *chunk = getChunk(
		std::floor((float)x / CHUNK_WIDTH),
		std::floor((float)y / CHUNK_HEIGHT),
		std::floor((float)z / CHUNK_DEPTH)
	);

	if (chunk)
		chunk->setBlock(x & (CHUNK_WIDTH - 1), y & (CHUNK_HEIGHT - 1), z & (CHUNK_DEPTH - 1), id);
}
