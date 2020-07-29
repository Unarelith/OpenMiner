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
#include "FastNoise.hpp"
#include "Heightmap.hpp"
#include "World.hpp"

void HeightmapChunk::generate() {
	FastNoise noise;
	noise.SetNoiseType(FastNoise::NoiseType::SimplexFractal);
	noise.SetFrequency(1 / 256.0f);
	noise.SetFractalOctaves(4);

	for(int y = 0 ; y < CHUNK_DEPTH ; y++) {
		for(int x = 0 ; x < CHUNK_WIDTH ; x++) {
			double n = noise.GetNoise(-x - m_x * CHUNK_WIDTH, y + m_y * CHUNK_DEPTH);
			m_map[x + y * CHUNK_WIDTH] = 10 + n * 20;
		}
	}
}

s32 HeightmapChunk::landHeightAt(s8 x, s8 y) const {
	return m_map[x + y * CHUNK_WIDTH];
}

void HeightmapChunk::setLandHeight(s8 x, s8 y, s32 height) {
	m_map[x + y * CHUNK_WIDTH] = height;
}

HeightmapChunk &Heightmap::getOrCreateChunk(s32 chunkX, s32 chunkY) {
	HeightmapChunk *chunk = nullptr;

	auto it = m_chunks.find({chunkX, chunkY});
	if (it == m_chunks.end()) {
		m_chunks.emplace(gk::Vector2i{chunkX, chunkY}, HeightmapChunk{chunkX, chunkY});

		chunk = &m_chunks.at({chunkX, chunkY});
		chunk->generate();
	}
	else
		chunk = &it->second;

	return *chunk;
}

int Heightmap::getHighestBlockAt(s32 blockX, s32 blockY) {
	s32 chunkX = (blockX & -CHUNK_WIDTH) / CHUNK_WIDTH;
	s32 chunkY = (blockY & -CHUNK_DEPTH) / CHUNK_DEPTH;

	s32 blockZ = getOrCreateChunk(chunkX, chunkY).landHeightAt(
		gk::pmod(blockX, CHUNK_WIDTH),
		gk::pmod(blockY, CHUNK_DEPTH)
	);

	return blockZ;
}

int Heightmap::getHighestChunkAt(s32 blockX, s32 blockY) {
	return (getHighestBlockAt(blockX, blockY) & -CHUNK_HEIGHT) / CHUNK_HEIGHT;
}

