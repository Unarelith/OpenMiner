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
#include "common/world/Heightmap.hpp"
#include "common/world/World.hpp"

void HeightmapChunk::generate() {
	for(int y = 0 ; y < CHUNK_DEPTH ; y++) {
		for(int x = 0 ; x < CHUNK_WIDTH ; x++) {
			double n1 = m_heightmap.noise1.GetNoise<double>(x + m_x * CHUNK_WIDTH, y + m_y * CHUNK_DEPTH);
			double n2 = m_heightmap.noise2.GetNoise<double>(x + m_x * CHUNK_WIDTH, y + m_y * CHUNK_DEPTH);
			double n3 = m_heightmap.noise3.GetNoise<double>(x + m_x * CHUNK_WIDTH, y + m_y * CHUNK_DEPTH);
			double n4 = m_heightmap.noise4.GetNoise<double>(x + m_x * CHUNK_WIDTH, y + m_y * CHUNK_DEPTH);
			m_map[x + y * CHUNK_WIDTH] = s32((n1 + (n2 * n3 * (n4 * 2 - 1))) * 64 + 64);
		}
	}
}

s32 HeightmapChunk::landHeightAt(s8 x, s8 y) const {
	return m_map[x + y * CHUNK_WIDTH];
}

void HeightmapChunk::setLandHeight(s8 x, s8 y, s32 height) {
	m_map[x + y * CHUNK_WIDTH] = height;
}

Heightmap::Heightmap(s32 seed) {
	noise1.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	noise1.SetFractalType(FastNoiseLite::FractalType_FBm);
	noise1.SetFractalOctaves(4);
	noise1.SetFrequency(1 / 256.0f);

	noise2.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	noise2.SetFractalType(FastNoiseLite::FractalType_FBm);
	noise2.SetFractalOctaves(4);
	noise2.SetFrequency(1 / 256.0f);

	noise3.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	noise3.SetFractalType(FastNoiseLite::FractalType_FBm);
	noise3.SetFractalOctaves(4);
	noise3.SetFrequency(1 / 256.0f);

	noise4.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	// noise4.SetFractalType(FastNoiseLite::FractalType_FBm);
	// noise4.SetFractalOctaves(1);
	noise4.SetFrequency(1 / 1024.0f);

	setSeed(seed);
}

HeightmapChunk &Heightmap::getOrCreateChunk(s32 chunkX, s32 chunkY) {
	HeightmapChunk *chunk = nullptr;

	auto it = m_chunks.find({chunkX, chunkY});
	if (it == m_chunks.end()) {
		m_chunks.emplace(Vector2i{chunkX, chunkY}, HeightmapChunk{*this, chunkX, chunkY});

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
		s8(math::pmod(blockX, CHUNK_WIDTH)),
		s8(math::pmod(blockY, CHUNK_DEPTH))
	);

	return blockZ;
}

int Heightmap::getHighestChunkAt(s32 blockX, s32 blockY) {
	return (getHighestBlockAt(blockX, blockY) & -CHUNK_HEIGHT) / CHUNK_HEIGHT;
}

void Heightmap::setSeed(s32 seed) {
	noise1.SetSeed(seed);
	noise2.SetSeed(seed);
	noise3.SetSeed(seed);
	noise4.SetSeed(seed);
}
