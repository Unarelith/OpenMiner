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
#ifndef HEIGHTMAP_HPP_
#define HEIGHTMAP_HPP_

#include <cstring>
#include <unordered_map>

#include <gk/core/IntTypes.hpp>
#include <gk/core/Vector2.hpp>

#include "EngineConfig.hpp"
#include "FastNoise.hpp"

class Heightmap;

class HeightmapChunk {
	public:
		HeightmapChunk(Heightmap &heightmap, s32 x, s32 y)
			: m_heightmap(heightmap), m_x(x), m_y(y) {
				std::memset(m_map, 0, CHUNK_WIDTH * CHUNK_DEPTH * sizeof(s32));
		}

		void generate();

		s32 landHeightAt(s8 x, s8 y) const;
		void setLandHeight(s8 x, s8 y, s32 height);

	private:
		Heightmap &m_heightmap;

		s32 m_x = 0;
		s32 m_y = 0;

		s32 m_map[CHUNK_WIDTH * CHUNK_DEPTH];
};

class Heightmap {
	public:
		Heightmap(s32 seed);

		HeightmapChunk &getOrCreateChunk(s32 chunkX, s32 chunkY);

		int getHighestBlockAt(s32 blockX, s32 blockY);
		int getHighestChunkAt(s32 blockX, s32 blockY);

		void setSeed(s32 seed);

		FastNoise noise1;
		FastNoise noise2;
		FastNoise noise3;
		FastNoise noise4;

	private:
		std::unordered_map<gk::Vector2i, HeightmapChunk> m_chunks;
};

#endif // HEIGHTMAP_HPP_
