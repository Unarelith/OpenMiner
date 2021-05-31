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
#include "ChunkBuilder.hpp"
#include "Registry.hpp"

struct ChunkData {
	s32 x, y, z;
	Chunk::DataArray data;
	ChunkLightmap::LightMapArray lightData;

	ChunkData *neighbours[6]{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};

	void loadFromChunk(const Chunk &chunk) {
		x = chunk.x();
		y = chunk.y();
		z = chunk.z();

		std::memcpy(&data, &chunk.data(), sizeof(Chunk::DataArray));
		std::memcpy(&lightData, &chunk.lightmap().data(), sizeof(ChunkLightmap::LightMapArray));
	}

	u16 getBlockID(s8f x, s8f y, s8f z) const {
		if(x < 0)              return neighbours[0] ? neighbours[0]->getBlockID(x + Chunk::width, y, z) : 0;
		if(x >= Chunk::width)  return neighbours[1] ? neighbours[1]->getBlockID(x - Chunk::width, y, z) : 0;
		if(y < 0)              return neighbours[2] ? neighbours[2]->getBlockID(x, y + Chunk::depth, z) : 0;
		if(y >= Chunk::depth)  return neighbours[3] ? neighbours[3]->getBlockID(x, y - Chunk::depth, z) : 0;
		if(z < 0)              return neighbours[4] ? neighbours[4]->getBlockID(x, y, z + Chunk::height) : 0;
		if(z >= Chunk::height) return neighbours[5] ? neighbours[5]->getBlockID(x, y, z - Chunk::height) : 0;

		return data[z][y][x] & 0xffff;
	}

	u16 getBlockParam(s8f x, s8f y, s8f z) const {
		if(x < 0)              return neighbours[0] ? neighbours[0]->getBlockParam(x + Chunk::width, y, z) : 0;
		if(x >= Chunk::width)  return neighbours[1] ? neighbours[1]->getBlockParam(x - Chunk::width, y, z) : 0;
		if(y < 0)              return neighbours[2] ? neighbours[2]->getBlockParam(x, y + Chunk::depth, z) : 0;
		if(y >= Chunk::depth)  return neighbours[3] ? neighbours[3]->getBlockParam(x, y - Chunk::depth, z) : 0;
		if(z < 0)              return neighbours[4] ? neighbours[4]->getBlockParam(x, y, z + Chunk::height) : 0;
		if(z >= Chunk::height) return neighbours[5] ? neighbours[5]->getBlockParam(x, y, z - Chunk::height) : 0;

		return (data[z][y][x] >> 16) & 0xffff;
	}

	u8 getTorchlight(s8f x, s8f y, s8f z) const {
		if(x < 0)              return neighbours[0] ? neighbours[0]->getTorchlight(x + Chunk::width, y, z) : 0;
		if(x >= Chunk::width)  return neighbours[1] ? neighbours[1]->getTorchlight(x - Chunk::width, y, z) : 0;
		if(y < 0)              return neighbours[2] ? neighbours[2]->getTorchlight(x, y + Chunk::depth, z) : 0;
		if(y >= Chunk::depth)  return neighbours[3] ? neighbours[3]->getTorchlight(x, y - Chunk::depth, z) : 0;
		if(z < 0)              return neighbours[4] ? neighbours[4]->getTorchlight(x, y, z + Chunk::height) : 0;
		if(z >= Chunk::height) return neighbours[5] ? neighbours[5]->getTorchlight(x, y, z - Chunk::height) : 0;

		return lightData[z][y][x] & 0xf;
	}

	u8 getSunlight(s8f x, s8f y, s8f z) const {
		if(x < 0)              return neighbours[0] ? neighbours[0]->getSunlight(x + Chunk::width, y, z) : 15;
		if(x >= Chunk::width)  return neighbours[1] ? neighbours[1]->getSunlight(x - Chunk::width, y, z) : 15;
		if(y < 0)              return neighbours[2] ? neighbours[2]->getSunlight(x, y + Chunk::depth, z) : 15;
		if(y >= Chunk::depth)  return neighbours[3] ? neighbours[3]->getSunlight(x, y - Chunk::depth, z) : 15;
		if(z < 0)              return neighbours[4] ? neighbours[4]->getSunlight(x, y, z + Chunk::height) : 15;
		if(z >= Chunk::height) return neighbours[5] ? neighbours[5]->getSunlight(x, y, z - Chunk::height) : 15;

		return (lightData[z][y][x] >> 4) & 0xf;
	}

	const BlockState *getBlockState(s8f x, s8f y, s8f z) const {
		if(x < 0)              return neighbours[0] ? neighbours[0]->getBlockState(x + Chunk::width, y, z) : nullptr;
		if(x >= Chunk::width)  return neighbours[1] ? neighbours[1]->getBlockState(x - Chunk::width, y, z) : nullptr;
		if(y < 0)              return neighbours[2] ? neighbours[2]->getBlockState(x, y + Chunk::depth, z) : nullptr;
		if(y >= Chunk::depth)  return neighbours[3] ? neighbours[3]->getBlockState(x, y - Chunk::depth, z) : nullptr;
		if(z < 0)              return neighbours[4] ? neighbours[4]->getBlockState(x, y, z + Chunk::height) : nullptr;
		if(z >= Chunk::height) return neighbours[5] ? neighbours[5]->getBlockState(x, y, z - Chunk::height) : nullptr;

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
	using VerticesArray = std::array<std::vector<Vertex>, ChunkBuilder::layers>;

	ChunkData chunkData;

	std::array<std::optional<ChunkData>, 6> neighbourData;

	VerticesArray vertices;

	const TextureAtlas *textureAtlas;
};

class ClientWorld;

class ChunkMeshBuilder {
	public:
		ChunkMeshBuilder(ClientWorld &world) : m_world(world) {}

		void addMeshBuildingJob(const Chunk &chunk, const TextureAtlas &textureAtlas);

		void update();

		enum Layer {
			Solid,
			NoMipMap,
			Flora,
			Glass,
			Liquid,

			Count
		};

	private:
		static void addCross(s8f x, s8f y, s8f z, ChunkMeshBuildingJob &job, const BlockState &blockState);

		static void addCube(s8f x, s8f y, s8f z, ChunkMeshBuildingJob &job, const BlockState &blockState, u16 blockParam);
		static void addCubeFace(s8f x, s8f y, s8f z, s8f f, ChunkMeshBuildingJob &job,
		             const BlockState &blockState,
		             const gk::Vector3i &normal, const glm::vec3 *const vertexPos[4],
		             const gk::Vector3i *const neighbourOfs[4]);

		enum class Light {
			Sun,
			Torch
		};

		static u8 getAmbientOcclusion(s8f x, s8f y, s8f z, const gk::Vector3i &offset,
		                              const gk::Vector3i &normal, const ChunkData &chunk);

		static u8 getLightForVertex(Light light, s8f x, s8f y, s8f z, const gk::Vector3i &offset,
		                            const gk::Vector3i &normal, const ChunkData &chunk);

		ClientWorld &m_world;

		thread::ThreadPool m_threadPool;

		std::vector<thread::ThreadPool::TaskFuture<ChunkMeshBuildingJob>> m_futures;
};

#endif // CHUNKMESHBUILDER_HPP_
