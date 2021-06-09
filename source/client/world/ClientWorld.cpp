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
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>

#include <gk/gl/GLCheck.hpp>
#include <gk/gl/Shader.hpp>
#include <gk/resource/ResourceHandler.hpp>

#include "ClientCommandHandler.hpp"
#include "ClientPlayer.hpp"
#include "ClientWorld.hpp"
#include "Events.hpp"
#include "Registry.hpp"
#include "Sky.hpp"
#include "TextureAtlas.hpp"
#include "World.hpp"

ClientWorld::ClientWorld() : m_textureAtlas(gk::ResourceHandler::getInstance().get<TextureAtlas>("atlas-blocks"))
{
}

void ClientWorld::update(bool allowWorldReload) {
	World::update();

	u64 time = std::time(nullptr);
	if (time > ClientChunk::chunkUpdateTime) {
		ClientChunk::chunkUpdatesPerSec = ClientChunk::chunkUpdateCounter;
		ClientChunk::chunkUpdateCounter = 0;
		ClientChunk::chunkUpdateTime = time;
	}

	for (auto it = m_chunksToRemove.begin() ; it != m_chunksToRemove.end() ;) {
		removeChunk(*it);
		it = m_chunksToRemove.erase(it);
	}

	if (allowWorldReload) {
		if (World::isReloadRequested) {
			for (auto &[pos, chunk] : m_chunks) {
				chunk->setChanged();
				addChunkToUpdate(chunk.get());
			}

			World::isReloadRequested = false;
		}
	}

	requestChunkMeshing();

	m_scene.update();

	m_chunkMeshBuilder.update();
}

void ClientWorld::requestChunkMeshing() {
	for (auto it = m_chunksToMesh.begin() ; it != m_chunksToMesh.end() ; ) {
		ClientChunk *chunk = (ClientChunk *)getChunk(it->second.x, it->second.y, it->second.z);
		if(chunk && !chunk->isReadyForMeshing() && chunk->areAllNeighboursLoaded()) {
			chunk->setReadyForMeshing(true);
			chunk->setChanged();
			addChunkToUpdate(chunk);

			// gkDebug() << "Chunk at" << ux << uy << uz << "is ready for meshing";
		}

		it = m_chunksToMesh.erase(it);
	}
}

void ClientWorld::checkPlayerChunk(double playerX, double playerY, double playerZ) {
	int pcx = (int)std::floor(playerX / CHUNK_WIDTH);
	int pcy = (int)std::floor(playerY / CHUNK_DEPTH);
	int pcz = (int)std::floor(playerZ / CHUNK_HEIGHT);

	ClientChunk *chunk = (ClientChunk *)getChunk(pcx, pcy, pcz);
	if (!chunk) {
		m_chunks.emplace(gk::Vector3i{pcx, pcy, pcz}, new ClientChunk(pcx, pcy, pcz, *m_dimension, *this, m_textureAtlas));
	}
}

void ClientWorld::clear() {
	m_chunks.clear();
	m_scene.registry().clear();
}

void ClientWorld::changeDimension(u16 dimensionID) {
	World::clearUpdateQueues();

	const Dimension &dimension = Registry::getInstance().getDimension(dimensionID);
	m_dimension = &dimension;

	const Sky &sky = Registry::getInstance().getSkyFromStringID(dimension.sky());
	m_sky = &sky;

	// glCheck(glClearColor(sky.color().r, sky.color().g, sky.color().b, sky.color().a));
}

void ClientWorld::receiveChunkData(Network::Packet &packet) {
	if (!m_dimension) return;

	s32 cx, cy, cz;
	packet >> cx >> cy >> cz;

	// Get the chunk from the map or create it if it doesn't exist
	ClientChunk *chunk = (ClientChunk *)getChunk(cx, cy, cz);
	if (!chunk) {
		auto it = m_chunks.emplace(gk::Vector3i{cx, cy, cz}, new ClientChunk(cx, cy, cz, *m_dimension, *this, m_textureAtlas));
		chunk = it.first->second.get();
	}

	linkChunkNeighbours(chunk);

	// Receive chunk data
	// bool hasUpdatedChunk = false;
	for (u8 z = 0 ; z < CHUNK_HEIGHT ; ++z) {
		for (u8 y = 0 ; y < CHUNK_DEPTH ; ++y) {
			for (u8 x = 0 ; x < CHUNK_WIDTH ; ++x) {
				u32 block;
				u8 light;

				packet >> block >> light;

				chunk->setDataRaw(x, y, z, block);
				chunk->lightmap().setLightDataRaw(x, y, z, light);

				// bool updatedBlock = chunk->setBlockRaw(x, y, z, block & 0xffff);
				// bool updatedData = chunk->setData(x, y, z, block >> 16);
				// bool updatedLight = chunk->lightmap().setLightData(x, y, z, light);
				// if (updatedBlock || updatedData || updatedLight)
				// 	hasUpdatedChunk = true;
			}
		}
	}

	addChunkToUpdate(chunk);

	// if (!hasUpdatedChunk)
	// 	gkWarning() << "Received chunk at" << cx << cy << cz << "without any updates";

	++chunk->debugTimesReceived;

	if (!chunk->isInitialized()) {
		auto addSurroundingChunkToUpdate = [this, chunk](u8 i) {
			Chunk *surroundingChunk = chunk->getSurroundingChunk(i);
			if (surroundingChunk) {
				surroundingChunk->setChanged();
				addChunkToUpdate(surroundingChunk);
			}
		};

		addSurroundingChunkToUpdate(0);
		addSurroundingChunkToUpdate(1);
		addSurroundingChunkToUpdate(2);
		addSurroundingChunkToUpdate(3);
		addSurroundingChunkToUpdate(4);
		addSurroundingChunkToUpdate(5);

		chunk->setInitialized(true);
	}

	if (m_eventHandler)
		m_eventHandler->emplaceEvent<ChunkCreatedEvent>(gk::Vector3i{cx, cy, cz}, true);

	// gkDebug() << "Chunk at" << cx << cy << cz << "received";
}

void ClientWorld::removeChunk(const gk::Vector3i &chunkPos) {
	auto it = m_chunks.find(chunkPos);
	if (it == m_chunks.end()) {
		gkWarning() << "Tried to remove unloaded chunk at" << chunkPos.x << chunkPos.y << chunkPos.z;
		return;
	}

	ClientChunk *surroundingChunks[6] = {
		(ClientChunk *)it->second->getSurroundingChunk(0),
		(ClientChunk *)it->second->getSurroundingChunk(1),
		(ClientChunk *)it->second->getSurroundingChunk(2),
		(ClientChunk *)it->second->getSurroundingChunk(3),
		(ClientChunk *)it->second->getSurroundingChunk(4),
		(ClientChunk *)it->second->getSurroundingChunk(5)
	};

	if (m_eventHandler)
		m_eventHandler->emplaceEvent<ChunkRemovedEvent>(gk::Vector3i{it->second->x(), it->second->y(), it->second->z()});

	m_chunks.erase(it);

	for (u8 i = 0 ; i < 6 ; ++i)
		if (surroundingChunks[i])
			surroundingChunks[i]->setSurroundingChunk((i % 2 == 0) ? i + 1 : i - 1, nullptr);

	// gkDebug() << "Chunk at" << chunkPos.x << chunkPos.y << chunkPos.z << "removed";
}

Chunk *ClientWorld::getChunk(int cx, int cy, int cz) const {
	auto it = m_chunks.find({cx, cy, cz});
	if (it == m_chunks.end())
		return nullptr;

	return it->second.get();
}

void ClientWorld::linkChunkNeighbours(ClientChunk *chunk) {
	gk::Vector3i surroundingChunks[6] = {
		{chunk->x() - 1, chunk->y(),     chunk->z()},
		{chunk->x() + 1, chunk->y(),     chunk->z()},
		{chunk->x(),     chunk->y() - 1, chunk->z()},
		{chunk->x(),     chunk->y() + 1, chunk->z()},
		{chunk->x(),     chunk->y(),     chunk->z() - 1},
		{chunk->x(),     chunk->y(),     chunk->z() + 1},
	};

	// Create entries in the map for surrounding chunks
	for (u8 i = 0 ; i < 6 ; ++i) {
		const s32 scx = surroundingChunks[i].x;
		const s32 scy = surroundingChunks[i].y;
		const s32 scz = surroundingChunks[i].z;

		ClientChunk *neighbour = (ClientChunk *)getChunk(scx, scy, scz);
		if (neighbour) {
			chunk->setSurroundingChunk(i, neighbour);
			neighbour->setSurroundingChunk((i % 2 == 0) ? i + 1 : i - 1, chunk);
		}
	}
}

void ClientWorld::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	if (!target.getView() || !m_camera) {
		gkError() << "Trying to draw world without a camera";
		return;
	}

	states.vertexAttributes = VertexAttribute::All;

	gk::Shader::bind(states.shader);

	states.shader->setUniform("u_renderDistance", Config::renderDistance * CHUNK_WIDTH);

	if (m_sky)
		states.shader->setUniform("u_fogColor", m_sky->fogColor());

	gk::Shader::bind(nullptr);

	// Changing the values sent to the GPU to double precision is suicidal,
	// performance wise, if possible at all. Therefore we want to keep the
	// GL rendering numbers in single precision format. But that introduces
	// an issue at larger coordinates, because the precision of floats
	// quickly degrades as the numbers grow, with a random wobbling being
	// very noticeable at e.g. coordinates >= 65536 or so, and the waving
	// leaves effect being very jerky in comparison with the effect near the
	// origin.
	//
	// To gain rendering precision, we subtract the camera position from the
	// coordinates of the models to be rendered, to make them all small in
	// relation to the camera, prior to converting them to floats. Then the
	// camera itself is moved to (0, 0, 0) for rendering purposes. Now the
	// vertex coordinates passed to the renderer are all small, and single
	// precision floats suffice for the drawing.

	gk::Vector3d cameraPos(m_camera->getDPosition());
	m_camera->setDPosition(0, 0, 0);  // Temporarily move the camera to the origin

	std::vector<std::pair<ClientChunk*, gk::Transform>> chunks;
	for(auto &it : m_chunks) {
		it.second->setHasBeenDrawn(false);

		gk::Transform tf = glm::translate(glm::mat4(1.0f),
		                                  glm::vec3(it.second->x() * CHUNK_WIDTH  - cameraPos.x,
		                                            it.second->y() * CHUNK_DEPTH  - cameraPos.y,
		                                            it.second->z() * CHUNK_HEIGHT - cameraPos.z));

		// Is the chunk close enough?
		glm::vec4 center = target.getView()->getViewTransform().getMatrix()
		                 * tf.getMatrix()
		                 * glm::vec4(CHUNK_WIDTH / 2, CHUNK_DEPTH / 2, CHUNK_HEIGHT / 2, 1);

		// Nope, too far, don't render it
		if(glm::length(center) > (Config::renderDistance + 1.f) * CHUNK_WIDTH) {
			if(floor(glm::length(center)) > (Config::renderDistance + 2.f) * CHUNK_WIDTH) {
				it.second->setTooFar(true);

				if (!it.second->isInitialized())
					m_chunksToRemove.emplace(gk::Vector3i{it.second->x(), it.second->y(), it.second->z()});
			}

			continue;
		}

		it.second->setTooFar(false);

		if (!it.second->isInitialized()) continue;

		// Is this chunk's centre on the screen?
		float d = glm::length2(center);
		center.x /= center.w;
		center.y /= center.w;

		// If it is behind the camera, don't bother drawing it.
		// Our screen coordinates are +X right, +Y up, and for a right-handed
		// coordinate system, depth must be negative Z, so anything with a
		// positive Z is behind the camera.
		if (center.z > CHUNK_MAXSIZE) {
			continue;
		}

		// If it is outside the screen, don't bother drawing it
		if (fabsf(center.x) > 1 + fabsf(CHUNK_HEIGHT * 2 / center.w)
		 || fabsf(center.y) > 1 + fabsf(CHUNK_HEIGHT * 2 / center.w)) {
			// FIXME: Disable this test; one that considers all eight corners of the chunk is needed instead.
			//continue;
		}

		// If this chunk is not initialized, skip it and request meshing
		if(!it.second->isReadyForMeshing()) {
			m_chunksToMesh.emplace(d, gk::Vector3i{it.second->x(), it.second->y(), it.second->z()});

			continue;
		}

		chunks.emplace_back(it.second.get(), tf);
	}

	for (u8 i = 0 ; i < ChunkMeshLayer::Count ; ++i) {
		for (auto &it : chunks) {
			states.transform = it.second;
			it.first->drawLayer(target, states, i);

			it.first->setHasBeenDrawn(true);
		}
	}

	m_camera->setDPosition(cameraPos);  // Restore the camera to its original position

	states.transform = gk::Transform::Identity;
	target.draw(m_scene, states);
}

