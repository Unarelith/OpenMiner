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
#ifndef CHUNKMESHBUILDER_HPP_
#define CHUNKMESHBUILDER_HPP_

#include <thread/ThreadPool.hpp>

#include "Chunk.hpp"
#include "ChunkMeshLayer.hpp"
#include "Registry.hpp"
#include "TextureAtlas.hpp"
#include "Vertex.hpp"

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
};

class ClientWorld;

class ChunkMeshBuilder {
	public:
		ChunkMeshBuilder(ClientWorld &world) : m_world(world) {}

		void addMeshBuildingJob(const Chunk &chunk, const TextureAtlas &textureAtlas);

		void update();

	private:
		static void addCross(s8f x, s8f y, s8f z, ChunkMeshBuildingJob &job, const BlockState &blockState);

		static void addCube(s8f x, s8f y, s8f z, ChunkMeshBuildingJob &job, const BlockState &blockState, u16 blockParam);
		static void addCubeFace(s8f x, s8f y, s8f z, s8f f, ChunkMeshBuildingJob &job,
		             const BlockState &blockState,
		             const gk::Vector3<s8f> &normal, const glm::vec3 *const vertexPos[4],
		             const gk::Vector3<s8f> *const neighbourOfs[4]);

		enum class Light {
			Sun,
			Torch
		};

		static u8 getAmbientOcclusion(s8f x, s8f y, s8f z, const gk::Vector3<s8f> &offset,
		                              const gk::Vector3<s8f> &normal, const ChunkData &chunk);

		static u8 getLightForVertex(Light light, s8f x, s8f y, s8f z, const gk::Vector3<s8f> &offset,
		                            const gk::Vector3<s8f> &normal, const ChunkData &chunk);

		ClientWorld &m_world;

		std::vector<thread::ThreadPool::TaskFuture<ChunkMeshBuildingJob>> m_futures;
};

#endif // CHUNKMESHBUILDER_HPP_
