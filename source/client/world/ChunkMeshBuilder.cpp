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
#include "client/core/ClientProfiler.hpp"
#include "client/graphics/TextureAtlas.hpp"
#include "client/world/ChunkMeshBuilder.hpp"
#include "client/world/ClientWorld.hpp"

#include "client/world/mesh/BlockMesher.hpp"
#include "client/world/mesh/XShapeMesher.hpp"

// NOTE: TextureAtlas and Registry are accessed from different threads.
//
//       This is not a problem currently since both shouldn’t change during the execution,
//       but if they do, a lot of problems will occur.
//
//       Also, Registry is accessed from server too when running a singleplayer game,
//       so if it changes during the execution, that would cause problems there too.
//
void ChunkMeshBuilder::addMeshBuildingJob(const Chunk &chunk, const TextureAtlas &textureAtlas) {
	OM_PROFILE_START("ChunkMeshBuilder::addMeshBuildingJob");

	// Creating the job (creates a copy of the chunk to send it to the thread)
	ChunkMeshBuildingJob *job = new ChunkMeshBuildingJob;
	job->textureAtlas = &textureAtlas;
	job->chunkData.loadFromChunk(chunk);

	// Send the job to the thread pool
	auto future = thread::DefaultThreadPool::submitJob(&ChunkMeshBuilder::buildChunkMesh, job);
	m_futures.emplace_back(std::move(future));

	OM_PROFILE_END("ChunkMeshBuilder::addMeshBuildingJob");
}

void ChunkMeshBuilder::update() {
	for (auto it = m_futures.begin() ; it != m_futures.end() ; ) {
		if (it->future().valid() && it->future().wait_for(std::chrono::milliseconds(0)) == std::future_status::ready) {
			ChunkMeshBuildingJob *job = it->get();

			ClientChunk *chunk = (ClientChunk *)m_world.getChunk(job->chunkData.x, job->chunkData.y, job->chunkData.z);
			if (chunk && job->totalVertexCount) {
				VertexBuffer &vbo = chunk->getVertexBuffer();

				vbo.init(nullptr, (u32)job->totalVertexCount * sizeof(Vertex));

				u64 offset = 0;
				for (u8 i = 0 ; i < ChunkMeshLayer::Count ; ++i) {
					u32 vertexCount = (u32)job->vertices[i].size();
					chunk->setVerticesCount(i, vertexCount);

					if (vertexCount) {
						job->vertices[i].shrink_to_fit();

						vbo.update(job->vertices[i].data(), vertexCount * sizeof(Vertex), (u32)offset);

						offset += vertexCount;
					}
				}
			}

			delete job;

			it = m_futures.erase(it);
		}
		else
			++it;
	}
}

ChunkMeshBuildingJob *ChunkMeshBuilder::buildChunkMesh(ChunkMeshBuildingJob *job) {
	// For each block, generate its vertices and add them to the list
	for (s8f z = 0 ; z < CHUNK_HEIGHT ; z++) {
		for (s8f y = 0 ; y < CHUNK_DEPTH ; y++) {
			for (s8f x = 0 ; x < CHUNK_WIDTH ; x++) {
				u16 blockID = job->chunkData.getBlockID(x, y, z);
				if (!blockID) continue;

				u16 blockParam = job->chunkData.getBlockParam(x, y, z);
				const BlockState &blockState = ChunkData::getBlockState(blockID, blockParam);

				if (blockState.drawType() == BlockDrawType::XShape)
					XShapeMesher::addCross(x, y, z, *job, blockState);
				else
					BlockMesher::addBlock(x, y, z, *job, blockState, blockParam);
			}
		}
	}

	return job;
}
