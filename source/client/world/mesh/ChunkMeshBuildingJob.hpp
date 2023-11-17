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
#ifndef CHUNKMESHBUILDINGJOB_HPP_
#define CHUNKMESHBUILDINGJOB_HPP_

#include "common/core/Registry.hpp"
#include "common/world/Chunk.hpp"

#include "client/graphics/TextureAtlas.hpp"
#include "client/graphics/Vertex.hpp"
#include "client/world/ChunkMeshLayer.hpp"

struct ChunkData {
	s32 x, y, z;
	u32 data[CHUNK_HEIGHT + 2][CHUNK_DEPTH + 2][CHUNK_WIDTH + 2];
	std::optional<u8> lightData[CHUNK_HEIGHT + 2][CHUNK_DEPTH + 2][CHUNK_WIDTH + 2];

	void loadFromChunk(const Chunk &chunk) {
		x = chunk.x();
		y = chunk.y();
		z = chunk.z();

		for (s8f z = -1 ; z <= CHUNK_HEIGHT ; ++z) {
			for (s8f y = -1 ; y <= CHUNK_DEPTH ; ++y) {
				for (s8f x = -1 ; x <= CHUNK_WIDTH ; ++x) {
					data[z + 1][y + 1][x + 1] = chunk.getFullBlock(x, y, z);
					lightData[z + 1][y + 1][x + 1] = chunk.lightmap().tryGetLightData(x, y, z);
				}
			}
		}
	}

	u16 getBlockID(s8f x, s8f y, s8f z) const {
		return u16(data[z + 1][y + 1][x + 1] & 0xffff);
	}

	u16 getBlockParam(s8f x, s8f y, s8f z) const {
		return u16((data[z + 1][y + 1][x + 1] >> 16) & 0xffff);
	}

	s8 getTorchlight(s8f x, s8f y, s8f z) const {
		auto light = lightData[z + 1][y + 1][x + 1];
		return light ? light.value() & 0xf : -1;
	}

	s8 getSunlight(s8f x, s8f y, s8f z) const {
		auto light = lightData[z + 1][y + 1][x + 1];
		return light ? (light.value() >> 4) & 0xf : -1;
	}

	const BlockState *getBlockState(s8f x, s8f y, s8f z) const {
		if (!lightData[z + 1][y + 1][x + 1]) return nullptr;

		u16 blockID = getBlockID(x, y, z);
		u16 blockParam = getBlockParam(x, y, z);

		return &getBlockState(blockID, blockParam);
	}

	static const BlockState &getBlockState(u16 blockID, u16 blockParam) {
		const Block &block = Registry::getInstance().getBlock(blockID);
		return block.getState(block.param().hasParam(BlockParam::State) ?
			block.param().getParam(BlockParam::State, blockParam) : 0);
	}
};

struct ChunkMeshBuildingJob {
	using VerticesArray = std::array<std::vector<Vertex>, ChunkMeshLayer::Count>;

	ChunkData chunkData;

	VerticesArray vertices;

	const TextureAtlas *textureAtlas;

	u64 totalVertexCount = 0;
};

#endif // CHUNKMESHBUILDINGJOB_HPP_
