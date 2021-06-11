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
#include <gk/gl/GLCheck.hpp>

#include "ClientChunk.hpp"
#include "ClientWorld.hpp"
#include "TextureAtlas.hpp"
#include "World.hpp"

u32 ClientChunk::chunkUpdatesPerSec = 0;
u32 ClientChunk::chunkUpdateCounter = 0;
u64 ClientChunk::chunkUpdateTime = 0;

ClientChunk::ClientChunk(s32 x, s32 y, s32 z, const Dimension &dimension, ClientWorld &world)
	: Chunk(x, y, z, (World &)world), m_world(world), m_dimension(dimension)
{
	m_vbo.layout().addAttribute(0, "coord3d", 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid *>(offsetof(Vertex, coord3d)));
	m_vbo.layout().addAttribute(1, "texCoord", 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid *>(offsetof(Vertex, texCoord)));
	m_vbo.layout().addAttribute(2, "color", 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid *>(offsetof(Vertex, color)));
	m_vbo.layout().addAttribute(3, "normal", 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid *>(offsetof(Vertex, normal)));
	m_vbo.layout().addAttribute(4, "lightValue", 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid *>(offsetof(Vertex, lightValue)));
	m_vbo.layout().addAttribute(5, "ambientOcclusion", 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid *>(offsetof(Vertex, ambientOcclusion)));
}

void ClientChunk::update() {
	m_lightmap.updateLights();

	if ((m_hasChanged || m_lightmap.hasChanged()) && m_isReadyForMeshing) {
		// gkDebug() << "Chunk update at" << m_x << m_y << m_z << "| D:" << m_hasChanged << "| L:" << m_lightmap.hasChanged();

		m_hasChanged = false;
		m_lightmap.resetChangedFlag();

		m_world.addChunkToProcess(this);
	}
}

void ClientChunk::process() {
	if (m_isReadyForMeshing) {
		m_world.buildChunk(*this);

		++ClientChunk::chunkUpdateCounter;
	}
}

void ClientChunk::onBlockPlaced(int x, int y, int z, const Block &) {
	auto addSurroundingChunkToProcess = [this](u8 i) {
		Chunk *surroundingChunk = m_surroundingChunks[i];
		if (surroundingChunk)
			m_world.addChunkToProcess(surroundingChunk);
	};

	if(x == 0         ) addSurroundingChunkToProcess(West);
	if(x == width  - 1) addSurroundingChunkToProcess(East);
	if(y == 0         ) addSurroundingChunkToProcess(South);
	if(y == depth  - 1) addSurroundingChunkToProcess(North);
	if(z == 0         ) addSurroundingChunkToProcess(Bottom);
	if(z == height - 1) addSurroundingChunkToProcess(Top);
}

bool ClientChunk::areAllNeighboursTooFar() const {
	return (!m_surroundingChunks[Chunk::West]   || ((ClientChunk *)m_surroundingChunks[Chunk::West])->isTooFar())
		&& (!m_surroundingChunks[Chunk::East]   || ((ClientChunk *)m_surroundingChunks[Chunk::East])->isTooFar())
		&& (!m_surroundingChunks[Chunk::South]  || ((ClientChunk *)m_surroundingChunks[Chunk::South])->isTooFar())
		&& (!m_surroundingChunks[Chunk::North]  || ((ClientChunk *)m_surroundingChunks[Chunk::North])->isTooFar())
		&& (!m_surroundingChunks[Chunk::Bottom] || ((ClientChunk *)m_surroundingChunks[Chunk::Bottom])->isTooFar())
		&& (!m_surroundingChunks[Chunk::Top]    || ((ClientChunk *)m_surroundingChunks[Chunk::Top])->isTooFar());
}

