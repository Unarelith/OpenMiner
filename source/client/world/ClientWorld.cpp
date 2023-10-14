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
#include <algorithm> // std::max

#include <gk/resource/ResourceHandler.hpp>

#include "ClientCommandHandler.hpp"
#include "ClientPlayer.hpp"
#include "ClientProfiler.hpp"
#include "ClientWorld.hpp"
#include "Events.hpp"
#include "Registry.hpp"
#include "Sky.hpp"
#include "TextureAtlas.hpp"
#include "World.hpp"

ClientWorld::ClientWorld()
	: m_textureAtlas(gk::ResourceHandler::getInstance().get<TextureAtlas>("atlas-blocks")),
	  m_chunkRenderer(m_textureAtlas)
{
	m_chunkRenderer.setOnChunkDeletionRequestedCallback([this](const gk::Vector3i &chunkPos) {
		m_chunksToRemove.emplace(chunkPos);
	});

	m_chunkRenderer.setOnMeshingRequestedCallback([this](float distance, const gk::Vector3i &chunkPos) {
		m_chunksToMesh.emplace(distance, chunkPos);
	});
}

void ClientWorld::update(bool allowWorldReload) {
	World::update();

	// Debug values only used in DebugOverlay
	u64 time = std::time(nullptr);
	if (time > ClientChunk::chunkUpdateTime) {
		ClientChunk::chunkUpdatesPerSec = ClientChunk::chunkUpdateCounter;
		ClientChunk::chunkUpdateCounter = 0;
		ClientChunk::chunkUpdateTime = time;
	}
	if (time > ClientChunk::chunkDrawTime) {
		ClientChunk::chunksRenderedPerFrame = ClientChunk::chunkDrawCounter / std::max<u64>(1, ClientChunk::frameCounter - ClientChunk::chunkDrawStartFrame);
		ClientChunk::chunkDrawCallPerFrame = ClientChunk::chunkDrawCallCounter / std::max<u64>(1, ClientChunk::frameCounter - ClientChunk::chunkDrawStartFrame);
		ClientChunk::chunkDrawCounter = 0;
		ClientChunk::chunkDrawCallCounter = 0;
		ClientChunk::chunkDrawTime = time;
		ClientChunk::chunkDrawStartFrame = ClientChunk::frameCounter;
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
		m_chunks.emplace(gk::Vector3i{pcx, pcy, pcz}, new ClientChunk(pcx, pcy, pcz, *m_dimension, *this));
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

	OM_PROFILE_START("ClientWorld::receiveChunkData");

	s32 cx, cy, cz;
	packet >> cx >> cy >> cz;

	// Get the chunk from the map or create it if it doesn't exist
	ClientChunk *chunk = (ClientChunk *)getChunk(cx, cy, cz);
	if (!chunk) {
		auto it = m_chunks.emplace(gk::Vector3i{cx, cy, cz}, new ClientChunk(cx, cy, cz, *m_dimension, *this));
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

	OM_PROFILE_END("ClientWorld::receiveChunkData");
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

void ClientWorld::draw(RenderTarget &target, RenderStates states) const {
	if (!target.getView() || !m_camera) {
		gkError() << "Trying to draw world without a camera";
		return;
	}

	OM_PROFILE_START("ClientWorld::draw");

	m_chunkRenderer.draw(target, states, m_chunks, *m_camera, m_sky);

	target.draw(m_scene, states);

	OM_PROFILE_END("ClientWorld::draw");
}

