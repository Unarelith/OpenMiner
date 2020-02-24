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
#include <cstring>

#include <gk/core/Debug.hpp>

#include "Chunk.hpp"
#include "ChunkLightmap.hpp"
#include "Registry.hpp"

ChunkLightmap::ChunkLightmap(Chunk *chunk) : m_chunk(chunk) {
	std::memset(m_lightMap, 0, sizeof(m_lightMap));
}

void ChunkLightmap::addTorchlight(int x, int y, int z, int val) {
	if(x < 0)             { if(m_chunk->getSurroundingChunk(0)) m_chunk->getSurroundingChunk(0)->lightmap().addTorchlight(x + CHUNK_WIDTH, y, z, val); return; }
	if(x >= CHUNK_WIDTH)  { if(m_chunk->getSurroundingChunk(1)) m_chunk->getSurroundingChunk(1)->lightmap().addTorchlight(x - CHUNK_WIDTH, y, z, val); return; }
	if(y < 0)             { if(m_chunk->getSurroundingChunk(2)) m_chunk->getSurroundingChunk(2)->lightmap().addTorchlight(x, y + CHUNK_DEPTH, z, val); return; }
	if(y >= CHUNK_DEPTH)  { if(m_chunk->getSurroundingChunk(3)) m_chunk->getSurroundingChunk(3)->lightmap().addTorchlight(x, y - CHUNK_DEPTH, z, val); return; }
	if(z < 0)             { if(m_chunk->getSurroundingChunk(4)) m_chunk->getSurroundingChunk(4)->lightmap().addTorchlight(x, y, z + CHUNK_HEIGHT, val); return; }
	if(z >= CHUNK_HEIGHT) { if(m_chunk->getSurroundingChunk(5)) m_chunk->getSurroundingChunk(5)->lightmap().addTorchlight(x, y, z - CHUNK_HEIGHT, val); return; }

	setTorchlight(x, y, z, val);
	m_torchlightBfsQueue.emplace(x, y, z);
}

void ChunkLightmap::addSunlight(int x, int y, int z, int val) {
	if(x < 0)             { if(m_chunk->getSurroundingChunk(0)) m_chunk->getSurroundingChunk(0)->lightmap().addSunlight(x + CHUNK_WIDTH, y, z, val); return; }
	if(x >= CHUNK_WIDTH)  { if(m_chunk->getSurroundingChunk(1)) m_chunk->getSurroundingChunk(1)->lightmap().addSunlight(x - CHUNK_WIDTH, y, z, val); return; }
	if(y < 0)             { if(m_chunk->getSurroundingChunk(2)) m_chunk->getSurroundingChunk(2)->lightmap().addSunlight(x, y + CHUNK_DEPTH, z, val); return; }
	if(y >= CHUNK_DEPTH)  { if(m_chunk->getSurroundingChunk(3)) m_chunk->getSurroundingChunk(3)->lightmap().addSunlight(x, y - CHUNK_DEPTH, z, val); return; }
	if(z < 0)             { if(m_chunk->getSurroundingChunk(4)) m_chunk->getSurroundingChunk(4)->lightmap().addSunlight(x, y, z + CHUNK_HEIGHT, val); return; }
	if(z >= CHUNK_HEIGHT) { if(m_chunk->getSurroundingChunk(5)) m_chunk->getSurroundingChunk(5)->lightmap().addSunlight(x, y, z - CHUNK_HEIGHT, val); return; }

	setSunlight(x, y, z, val);
	m_sunlightBfsQueue.emplace(x, y, z);
}

void ChunkLightmap::removeTorchlight(int x, int y, int z) {
	if(x < 0)             { if(m_chunk->getSurroundingChunk(0)) m_chunk->getSurroundingChunk(0)->lightmap().removeTorchlight(x + CHUNK_WIDTH, y, z); return; }
	if(x >= CHUNK_WIDTH)  { if(m_chunk->getSurroundingChunk(1)) m_chunk->getSurroundingChunk(1)->lightmap().removeTorchlight(x - CHUNK_WIDTH, y, z); return; }
	if(y < 0)             { if(m_chunk->getSurroundingChunk(2)) m_chunk->getSurroundingChunk(2)->lightmap().removeTorchlight(x, y + CHUNK_DEPTH, z); return; }
	if(y >= CHUNK_DEPTH)  { if(m_chunk->getSurroundingChunk(3)) m_chunk->getSurroundingChunk(3)->lightmap().removeTorchlight(x, y - CHUNK_DEPTH, z); return; }
	if(z < 0)             { if(m_chunk->getSurroundingChunk(4)) m_chunk->getSurroundingChunk(4)->lightmap().removeTorchlight(x, y, z + CHUNK_HEIGHT); return; }
	if(z >= CHUNK_HEIGHT) { if(m_chunk->getSurroundingChunk(5)) m_chunk->getSurroundingChunk(5)->lightmap().removeTorchlight(x, y, z - CHUNK_HEIGHT); return; }

	m_torchlightRemovalBfsQueue.emplace(x, y, z, getTorchlight(x, y, z));
	setTorchlight(x, y, z, 0);
}

void ChunkLightmap::removeSunlight(int x, int y, int z) {
	if(x < 0)             { if(m_chunk->getSurroundingChunk(0)) m_chunk->getSurroundingChunk(0)->lightmap().removeSunlight(x + CHUNK_WIDTH, y, z); return; }
	if(x >= CHUNK_WIDTH)  { if(m_chunk->getSurroundingChunk(1)) m_chunk->getSurroundingChunk(1)->lightmap().removeSunlight(x - CHUNK_WIDTH, y, z); return; }
	if(y < 0)             { if(m_chunk->getSurroundingChunk(2)) m_chunk->getSurroundingChunk(2)->lightmap().removeSunlight(x, y + CHUNK_DEPTH, z); return; }
	if(y >= CHUNK_DEPTH)  { if(m_chunk->getSurroundingChunk(3)) m_chunk->getSurroundingChunk(3)->lightmap().removeSunlight(x, y - CHUNK_DEPTH, z); return; }
	if(z < 0)             { if(m_chunk->getSurroundingChunk(4)) m_chunk->getSurroundingChunk(4)->lightmap().removeSunlight(x, y, z + CHUNK_HEIGHT); return; }
	if(z >= CHUNK_HEIGHT) { if(m_chunk->getSurroundingChunk(5)) m_chunk->getSurroundingChunk(5)->lightmap().removeSunlight(x, y, z - CHUNK_HEIGHT); return; }

	m_sunlightRemovalBfsQueue.emplace(x, y, z, getSunlight(x, y, z));
	setSunlight(x, y, z, 0);
}

bool ChunkLightmap::updateLights() {
	bool torchlightUpdated = updateTorchlight();
	bool sunlightUpdated = updateSunlight();

	return torchlightUpdated || sunlightUpdated;
}

bool ChunkLightmap::updateTorchlight() {
	bool lightUpdated = false;

	while (!m_torchlightRemovalBfsQueue.empty()) {
		LightRemovalNode node = m_torchlightRemovalBfsQueue.front();
		m_torchlightRemovalBfsQueue.pop();

		gk::Vector3i surroundingNodes[6] = {
			{node.x - 1, node.y,     node.z},
			{node.x + 1, node.y,     node.z},
			{node.x,     node.y - 1, node.z},
			{node.x,     node.y + 1, node.z},
			{node.x,     node.y,     node.z - 1},
			{node.x,     node.y,     node.z + 1},
		};

		for (const gk::Vector3i &surroundingNode : surroundingNodes) {
			int level = getTorchlight(surroundingNode.x, surroundingNode.y, surroundingNode.z);
			if (level != 0 && level < node.value) {
				setTorchlight(surroundingNode.x, surroundingNode.y, surroundingNode.z, 0);

				m_torchlightRemovalBfsQueue.emplace(surroundingNode.x, surroundingNode.y, surroundingNode.z, level);

				lightUpdated = true;
			}
			else if (level >= node.value) {
				m_torchlightBfsQueue.emplace(surroundingNode.x, surroundingNode.y, surroundingNode.z);

				lightUpdated = true;
			}
		}
	}

	while (!m_torchlightBfsQueue.empty()) {
		LightNode node = m_torchlightBfsQueue.front();
		m_torchlightBfsQueue.pop();

		// FIXME: This doesn't check if the block is an actual light source
		//        so if this block is a light source, it will just remove the light...

		// If this block is opaque, don't propagate the light
		// u16 block = m_chunk->getBlock(node.x, node.y, node.z);
		// const Block &block = Registry::getInstance().getBlock(blockID);
		// if (block.isOpaque()) {
		// 	setTorchlight(node.x, node.y, node.z, 0);
        //
		// 	lightUpdated = true; // FIXME
        //
		// 	continue;
		// }

		gk::Vector3i surroundingNodes[6] = {
			{node.x - 1, node.y,     node.z},
			{node.x + 1, node.y,     node.z},
			{node.x,     node.y - 1, node.z},
			{node.x,     node.y + 1, node.z},
			{node.x,     node.y,     node.z - 1},
			{node.x,     node.y,     node.z + 1},
		};

		u8 lightLevel = getTorchlight(node.x, node.y, node.z);
		for (const gk::Vector3i &surroundingNode : surroundingNodes) {
			if (getTorchlight(surroundingNode.x, surroundingNode.y, surroundingNode.z) + 2 <= lightLevel) {
				u16 blockID = m_chunk->getBlock(surroundingNode.x, surroundingNode.y, surroundingNode.z);
				const Block &block = Registry::getInstance().getBlock(blockID);
				if (!block.isOpaque()) {
					addTorchlight(surroundingNode.x, surroundingNode.y, surroundingNode.z, lightLevel - 1);

					lightUpdated = true;
				}
			}
		}
	}

	return lightUpdated;
}

bool ChunkLightmap::updateSunlight() {
	bool lightUpdated = false;

	while (!m_sunlightRemovalBfsQueue.empty()) {
		LightRemovalNode node = m_sunlightRemovalBfsQueue.front();
		m_sunlightRemovalBfsQueue.pop();

		gk::Vector3i surroundingNodes[6] = {
			{node.x - 1, node.y,     node.z},
			{node.x + 1, node.y,     node.z},
			{node.x,     node.y - 1, node.z},
			{node.x,     node.y + 1, node.z},
			{node.x,     node.y,     node.z - 1},
			{node.x,     node.y,     node.z + 1},
		};

		for (const gk::Vector3i &surroundingNode : surroundingNodes) {
			int level = getSunlight(surroundingNode.x, surroundingNode.y, surroundingNode.z);
			if ((level == 15 && surroundingNode.z == node.z - 1) || (level != 0 && level < node.value)) {
				setSunlight(surroundingNode.x, surroundingNode.y, surroundingNode.z, 0);

				m_sunlightRemovalBfsQueue.emplace(surroundingNode.x, surroundingNode.y, surroundingNode.z, level);

				lightUpdated = true;
			}
			else if (level >= node.value) {
				m_sunlightBfsQueue.emplace(surroundingNode.x, surroundingNode.y, surroundingNode.z);

				lightUpdated = true;
			}
		}
	}

	while (!m_sunlightBfsQueue.empty()) {
		LightNode node = m_sunlightBfsQueue.front();
		m_sunlightBfsQueue.pop();

		// If this block is opaque, don't propagate the light
		u16 blockID = m_chunk->getBlock(node.x, node.y, node.z);
		const Block &block = Registry::getInstance().getBlock(blockID);
		if (block.isOpaque()) {
			setSunlight(node.x, node.y, node.z, 0);

			// FIXME: This only reverts an addSunlight that added light in a non-generated chunk
			//        I should avoid setting the sunlight rather than reverting it
			lightUpdated = true;

			continue;
		}

		gk::Vector3i surroundingNodes[6] = {
			{node.x - 1, node.y,     node.z},
			{node.x + 1, node.y,     node.z},
			{node.x,     node.y - 1, node.z},
			{node.x,     node.y + 1, node.z},
			{node.x,     node.y,     node.z - 1},
			{node.x,     node.y,     node.z + 1},
		};

		u8 sunlightLevel = getSunlight(node.x, node.y, node.z);
		for (const gk::Vector3i &surroundingNode : surroundingNodes) {
			u8 neighbourSunlightLevel = getSunlight(surroundingNode.x, surroundingNode.y, surroundingNode.z);
			if (neighbourSunlightLevel + 2 <= sunlightLevel
			|| (sunlightLevel == 15 && neighbourSunlightLevel != 15 && surroundingNode.z == node.z - 1)) {
				u16 blockID = m_chunk->getBlock(surroundingNode.x, surroundingNode.y, surroundingNode.z);
				const Block &block = Registry::getInstance().getBlock(blockID);
				if (!block.isOpaque()) {
					if (sunlightLevel == 15 && surroundingNode.z == node.z - 1 && (!blockID || block.drawType() == BlockDrawType::Glass || block.drawType() == BlockDrawType::XShape)) {
						addSunlight(surroundingNode.x, surroundingNode.y, surroundingNode.z, sunlightLevel);

						lightUpdated = true;
					}
					else if (sunlightLevel == 15 && surroundingNode.z == node.z + 1)
						continue;
					else {
						addSunlight(surroundingNode.x, surroundingNode.y, surroundingNode.z, sunlightLevel - 1);

						// FIXME: If addSunlight changes something in a surrounding chunk
						//        then this flag should be set on this other chunk
						lightUpdated = true;
					}
				}
			}
		}
	}

	return lightUpdated;
}

u8 ChunkLightmap::getSunlight(int x, int y, int z) const {
	if(x < 0)             return m_chunk->getSurroundingChunk(0) ? m_chunk->getSurroundingChunk(0)->lightmap().getSunlight(x + CHUNK_WIDTH, y, z) : 15;
	if(x >= CHUNK_WIDTH)  return m_chunk->getSurroundingChunk(1) ? m_chunk->getSurroundingChunk(1)->lightmap().getSunlight(x - CHUNK_WIDTH, y, z) : 15;
	if(y < 0)             return m_chunk->getSurroundingChunk(2) ? m_chunk->getSurroundingChunk(2)->lightmap().getSunlight(x, y + CHUNK_DEPTH, z) : 15;
	if(y >= CHUNK_DEPTH)  return m_chunk->getSurroundingChunk(3) ? m_chunk->getSurroundingChunk(3)->lightmap().getSunlight(x, y - CHUNK_DEPTH, z) : 15;
	if(z < 0)             return m_chunk->getSurroundingChunk(4) ? m_chunk->getSurroundingChunk(4)->lightmap().getSunlight(x, y, z + CHUNK_HEIGHT) : 15;
	if(z >= CHUNK_HEIGHT) return m_chunk->getSurroundingChunk(5) ? m_chunk->getSurroundingChunk(5)->lightmap().getSunlight(x, y, z - CHUNK_HEIGHT) : 15;

	return (m_lightMap[z][y][x] >> 4) & 0xf;
}

u8 ChunkLightmap::getTorchlight(int x, int y, int z) const {
	if(x < 0)             return m_chunk->getSurroundingChunk(0) ? m_chunk->getSurroundingChunk(0)->lightmap().getTorchlight(x + CHUNK_WIDTH, y, z) : 0;
	if(x >= CHUNK_WIDTH)  return m_chunk->getSurroundingChunk(1) ? m_chunk->getSurroundingChunk(1)->lightmap().getTorchlight(x - CHUNK_WIDTH, y, z) : 0;
	if(y < 0)             return m_chunk->getSurroundingChunk(2) ? m_chunk->getSurroundingChunk(2)->lightmap().getTorchlight(x, y + CHUNK_DEPTH, z) : 0;
	if(y >= CHUNK_DEPTH)  return m_chunk->getSurroundingChunk(3) ? m_chunk->getSurroundingChunk(3)->lightmap().getTorchlight(x, y - CHUNK_DEPTH, z) : 0;
	if(z < 0)             return m_chunk->getSurroundingChunk(4) ? m_chunk->getSurroundingChunk(4)->lightmap().getTorchlight(x, y, z + CHUNK_HEIGHT) : 0;
	if(z >= CHUNK_HEIGHT) return m_chunk->getSurroundingChunk(5) ? m_chunk->getSurroundingChunk(5)->lightmap().getTorchlight(x, y, z - CHUNK_HEIGHT) : 0;

	return m_lightMap[z][y][x] & 0xf;
}

void ChunkLightmap::setLightData(int x, int y, int z, u8 val) {
	if (m_lightMap[z][y][x] != val) {
		m_lightMap[z][y][x] = val;

		m_chunk->setLightChanged(true);

		updateSurroundingChunks(x, y, z);
	}
}

void ChunkLightmap::setSunlight(int x, int y, int z, u8 val) {
	if(x < 0)             { if(m_chunk->getSurroundingChunk(0)) m_chunk->getSurroundingChunk(0)->lightmap().setSunlight(x + CHUNK_WIDTH, y, z, val); return; }
	if(x >= CHUNK_WIDTH)  { if(m_chunk->getSurroundingChunk(1)) m_chunk->getSurroundingChunk(1)->lightmap().setSunlight(x - CHUNK_WIDTH, y, z, val); return; }
	if(y < 0)             { if(m_chunk->getSurroundingChunk(2)) m_chunk->getSurroundingChunk(2)->lightmap().setSunlight(x, y + CHUNK_DEPTH, z, val); return; }
	if(y >= CHUNK_DEPTH)  { if(m_chunk->getSurroundingChunk(3)) m_chunk->getSurroundingChunk(3)->lightmap().setSunlight(x, y - CHUNK_DEPTH, z, val); return; }
	if(z < 0)             { if(m_chunk->getSurroundingChunk(4)) m_chunk->getSurroundingChunk(4)->lightmap().setSunlight(x, y, z + CHUNK_HEIGHT, val); return; }
	if(z >= CHUNK_HEIGHT) { if(m_chunk->getSurroundingChunk(5)) m_chunk->getSurroundingChunk(5)->lightmap().setSunlight(x, y, z - CHUNK_HEIGHT, val); return; }

	if ((m_lightMap[z][y][x] & 0xf0) != (val << 4)) {
		m_lightMap[z][y][x] = (m_lightMap[z][y][x] & 0xf) | (val << 4);

		m_chunk->setLightChanged(true);

		updateSurroundingChunks(x, y, z);
	}
};

void ChunkLightmap::setTorchlight(int x, int y, int z, u8 val) {
	if(x < 0)             { if(m_chunk->getSurroundingChunk(0)) m_chunk->getSurroundingChunk(0)->lightmap().setTorchlight(x + CHUNK_WIDTH, y, z, val); return; }
	if(x >= CHUNK_WIDTH)  { if(m_chunk->getSurroundingChunk(1)) m_chunk->getSurroundingChunk(1)->lightmap().setTorchlight(x - CHUNK_WIDTH, y, z, val); return; }
	if(y < 0)             { if(m_chunk->getSurroundingChunk(2)) m_chunk->getSurroundingChunk(2)->lightmap().setTorchlight(x, y + CHUNK_DEPTH, z, val); return; }
	if(y >= CHUNK_DEPTH)  { if(m_chunk->getSurroundingChunk(3)) m_chunk->getSurroundingChunk(3)->lightmap().setTorchlight(x, y - CHUNK_DEPTH, z, val); return; }
	if(z < 0)             { if(m_chunk->getSurroundingChunk(4)) m_chunk->getSurroundingChunk(4)->lightmap().setTorchlight(x, y, z + CHUNK_HEIGHT, val); return; }
	if(z >= CHUNK_HEIGHT) { if(m_chunk->getSurroundingChunk(5)) m_chunk->getSurroundingChunk(5)->lightmap().setTorchlight(x, y, z - CHUNK_HEIGHT, val); return; }

	if ((m_lightMap[z][y][x] & 0xf) != val << 4) {
		m_lightMap[z][y][x] = (m_lightMap[z][y][x] & 0xf0) | val;

		m_chunk->setLightChanged(true);

		updateSurroundingChunks(x, y, z);
	}
}

void ChunkLightmap::updateSurroundingChunks(int x, int y, int z) {
	if(x == 0                && m_chunk->getSurroundingChunk(Chunk::West))   { m_chunk->getSurroundingChunk(Chunk::West)->setLightChanged(true); }
	if(x == CHUNK_WIDTH - 1  && m_chunk->getSurroundingChunk(Chunk::East))  { m_chunk->getSurroundingChunk(Chunk::East)->setLightChanged(true); }
	if(y == 0                && m_chunk->getSurroundingChunk(Chunk::Bottom)) { m_chunk->getSurroundingChunk(Chunk::Bottom)->setLightChanged(true); }
	if(y == CHUNK_DEPTH - 1  && m_chunk->getSurroundingChunk(Chunk::Top))    { m_chunk->getSurroundingChunk(Chunk::Top)->setLightChanged(true); }
	if(z == 0                && m_chunk->getSurroundingChunk(Chunk::South))  { m_chunk->getSurroundingChunk(Chunk::South)->setLightChanged(true); }
	if(z == CHUNK_HEIGHT - 1 && m_chunk->getSurroundingChunk(Chunk::North))   { m_chunk->getSurroundingChunk(Chunk::North)->setLightChanged(true); }
}

