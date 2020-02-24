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

#include <gk/core/GameClock.hpp>

#include "Chunk.hpp"
#include "EngineConfig.hpp"
#include "Registry.hpp"

Chunk::Chunk(s32 x, s32 y, s32 z, World &world) : m_world(world) {
	m_x = x;
	m_y = y;
	m_z = z;

	std::memset(m_data, 0, sizeof(m_data));
}

u16 Chunk::getBlock(int x, int y, int z) const {
	if(x < 0)              return m_surroundingChunks[0] ? m_surroundingChunks[0]->getBlock(x + Chunk::width, y, z) : 0;
	if(x >= Chunk::width)  return m_surroundingChunks[1] ? m_surroundingChunks[1]->getBlock(x - Chunk::width, y, z) : 0;
	if(y < 0)              return m_surroundingChunks[2] ? m_surroundingChunks[2]->getBlock(x, y + Chunk::depth, z) : 0;
	if(y >= Chunk::depth)  return m_surroundingChunks[3] ? m_surroundingChunks[3]->getBlock(x, y - Chunk::depth, z) : 0;
	if(z < 0)              return m_surroundingChunks[4] ? m_surroundingChunks[4]->getBlock(x, y, z + Chunk::height) : 0;
	if(z >= Chunk::height) return m_surroundingChunks[5] ? m_surroundingChunks[5]->getBlock(x, y, z - Chunk::height) : 0;
	return m_data[z][y][x] & 0xffff;
}

u16 Chunk::getData(int x, int y, int z) const {
	if(x < 0)              return m_surroundingChunks[0] ? m_surroundingChunks[0]->getData(x + Chunk::width, y, z) : 0;
	if(x >= Chunk::width)  return m_surroundingChunks[1] ? m_surroundingChunks[1]->getData(x - Chunk::width, y, z) : 0;
	if(y < 0)              return m_surroundingChunks[2] ? m_surroundingChunks[2]->getData(x, y + Chunk::depth, z) : 0;
	if(y >= Chunk::depth)  return m_surroundingChunks[3] ? m_surroundingChunks[3]->getData(x, y - Chunk::depth, z) : 0;
	if(z < 0)              return m_surroundingChunks[4] ? m_surroundingChunks[4]->getData(x, y, z + Chunk::height) : 0;
	if(z >= Chunk::height) return m_surroundingChunks[5] ? m_surroundingChunks[5]->getData(x, y, z - Chunk::height) : 0;
	return (m_data[z][y][x] >> 16) & 0xffff;
}

void Chunk::setBlock(int x, int y, int z, u16 type) {
	if(x < 0)              { if(m_surroundingChunks[0]) m_surroundingChunks[0]->setBlock(x + Chunk::width, y, z, type); return; }
	if(x >= Chunk::width)  { if(m_surroundingChunks[1]) m_surroundingChunks[1]->setBlock(x - Chunk::width, y, z, type); return; }
	if(y < 0)              { if(m_surroundingChunks[2]) m_surroundingChunks[2]->setBlock(x, y + Chunk::depth, z, type); return; }
	if(y >= Chunk::depth)  { if(m_surroundingChunks[3]) m_surroundingChunks[3]->setBlock(x, y - Chunk::depth, z, type); return; }
	if(z < 0)              { if(m_surroundingChunks[4]) m_surroundingChunks[4]->setBlock(x, y, z + Chunk::height, type); return; }
	if(z >= Chunk::height) { if(m_surroundingChunks[5]) m_surroundingChunks[5]->setBlock(x, y, z - Chunk::height, type); return; }

	if (m_data[z][y][x] == type) return;

	const Block &block = Registry::getInstance().getBlock(type);
	if (block.canUpdate()) {
		m_tickingBlocks.emplace(x + y * width + z * (width * height), block);
	}
	else {
		auto it = m_tickingBlocks.find(x + y * width + z * (width * height));
		if (it != m_tickingBlocks.end())
			m_tickingBlocks.erase(it);
	}

	if (block.isLightSource())
		m_lightmap.addTorchlight(x, y, z, 14);
	else {
		m_lightmap.removeTorchlight(x, y, z);
		m_lightmap.removeSunlight(x, y, z);
	}

	onBlockPlaced(x, y, z, block);

	setBlockRaw(x, y, z, type);

	if(x == 0          && m_surroundingChunks[West])   { m_surroundingChunks[West]->m_hasChanged = true; }
	if(x == width - 1  && m_surroundingChunks[East])   { m_surroundingChunks[East]->m_hasChanged = true; }
	if(y == 0          && m_surroundingChunks[South])  { m_surroundingChunks[South]->m_hasChanged = true; }
	if(y == depth - 1  && m_surroundingChunks[North])  { m_surroundingChunks[North]->m_hasChanged = true; }
	if(z == 0          && m_surroundingChunks[Bottom]) { m_surroundingChunks[Bottom]->m_hasChanged = true; }
	if(z == height - 1 && m_surroundingChunks[Top])    { m_surroundingChunks[Top]->m_hasChanged = true; }
}

void Chunk::setData(int x, int y, int z, u16 data) {
	if(x < 0)              { if(m_surroundingChunks[0]) m_surroundingChunks[0]->setData(x + Chunk::width, y, z, data); return; }
	if(x >= Chunk::width)  { if(m_surroundingChunks[1]) m_surroundingChunks[1]->setData(x - Chunk::width, y, z, data); return; }
	if(y < 0)              { if(m_surroundingChunks[2]) m_surroundingChunks[2]->setData(x, y + Chunk::depth, z, data); return; }
	if(y >= Chunk::depth)  { if(m_surroundingChunks[3]) m_surroundingChunks[3]->setData(x, y - Chunk::depth, z, data); return; }
	if(z < 0)              { if(m_surroundingChunks[4]) m_surroundingChunks[4]->setData(x, y, z + Chunk::height, data); return; }
	if(z >= Chunk::height) { if(m_surroundingChunks[5]) m_surroundingChunks[5]->setData(x, y, z - Chunk::height, data); return; }

	m_data[z][y][x] &= 0xffff;
	m_data[z][y][x] |= (data << 16);

	m_hasChanged = true;
}

void Chunk::setBlockRaw(int x, int y, int z, u16 type) {
	if(x < 0)              { if(m_surroundingChunks[0]) m_surroundingChunks[0]->setBlockRaw(x + Chunk::width, y, z, type); return; }
	if(x >= Chunk::width)  { if(m_surroundingChunks[1]) m_surroundingChunks[1]->setBlockRaw(x - Chunk::width, y, z, type); return; }
	if(y < 0)              { if(m_surroundingChunks[2]) m_surroundingChunks[2]->setBlockRaw(x, y + Chunk::depth, z, type); return; }
	if(y >= Chunk::depth)  { if(m_surroundingChunks[3]) m_surroundingChunks[3]->setBlockRaw(x, y - Chunk::depth, z, type); return; }
	if(z < 0)              { if(m_surroundingChunks[4]) m_surroundingChunks[4]->setBlockRaw(x, y, z + Chunk::height, type); return; }
	if(z >= Chunk::height) { if(m_surroundingChunks[5]) m_surroundingChunks[5]->setBlockRaw(x, y, z - Chunk::height, type); return; }

	if (m_data[z][y][x] == type) return;

	if (type == 0) {
		auto it = m_blockData.find(gk::Vector3i{x, y, z});
		if (it != m_blockData.end())
			m_blockData.erase(it);
	}

	m_data[z][y][x] = type;

	m_hasChanged = true;
}

BlockData *Chunk::getBlockData(int x, int y, int z) const {
	if(x < 0)             return m_surroundingChunks[0] ? m_surroundingChunks[0]->getBlockData(x + CHUNK_WIDTH, y, z) : 0;
	if(x >= CHUNK_WIDTH)  return m_surroundingChunks[1] ? m_surroundingChunks[1]->getBlockData(x - CHUNK_WIDTH, y, z) : 0;
	if(y < 0)             return m_surroundingChunks[2] ? m_surroundingChunks[2]->getBlockData(x, y + CHUNK_DEPTH, z) : 0;
	if(y >= CHUNK_DEPTH)  return m_surroundingChunks[3] ? m_surroundingChunks[3]->getBlockData(x, y - CHUNK_DEPTH, z) : 0;
	if(z < 0)             return m_surroundingChunks[4] ? m_surroundingChunks[4]->getBlockData(x, y, z + CHUNK_HEIGHT) : 0;
	if(z >= CHUNK_HEIGHT) return m_surroundingChunks[5] ? m_surroundingChunks[5]->getBlockData(x, y, z - CHUNK_HEIGHT) : 0;

	gk::Vector3i pos{x, y, z};
	auto it = m_blockData.find(pos);
	if (it == m_blockData.end()) {
		return nullptr;
	}

	return it->second.get();
}

BlockData *Chunk::addBlockData(int x, int y, int z, int inventoryWidth, int inventoryHeight) {
	if(x < 0)             return m_surroundingChunks[0] ? m_surroundingChunks[0]->addBlockData(x + CHUNK_WIDTH, y, z) : 0;
	if(x >= CHUNK_WIDTH)  return m_surroundingChunks[1] ? m_surroundingChunks[1]->addBlockData(x - CHUNK_WIDTH, y, z) : 0;
	if(y < 0)             return m_surroundingChunks[2] ? m_surroundingChunks[2]->addBlockData(x, y + CHUNK_DEPTH, z) : 0;
	if(y >= CHUNK_DEPTH)  return m_surroundingChunks[3] ? m_surroundingChunks[3]->addBlockData(x, y - CHUNK_DEPTH, z) : 0;
	if(z < 0)             return m_surroundingChunks[4] ? m_surroundingChunks[4]->addBlockData(x, y, z + CHUNK_HEIGHT) : 0;
	if(z >= CHUNK_HEIGHT) return m_surroundingChunks[5] ? m_surroundingChunks[5]->addBlockData(x, y, z - CHUNK_HEIGHT) : 0;

	gk::Vector3i pos{x, y, z};
	auto it = m_blockData.find(pos);
	if (it == m_blockData.end()) {
		gk::Vector3i absolutePos{x + m_x * CHUNK_WIDTH, y + m_y * CHUNK_DEPTH, z + m_z * CHUNK_HEIGHT};
		BlockData *data = new BlockData{absolutePos, inventoryWidth, inventoryHeight};
		m_blockData.emplace(pos, data);
		return data;
	}

	return it->second.get();
}

bool Chunk::areAllNeighboursLoaded() const {
	return m_surroundingChunks[Chunk::West]
		&& m_surroundingChunks[Chunk::East]
		&& m_surroundingChunks[Chunk::South]
		&& m_surroundingChunks[Chunk::North]
		&& m_surroundingChunks[Chunk::Bottom]
		&& m_surroundingChunks[Chunk::Top];
}

bool Chunk::areAllNeighboursInitialized() const {
	return m_surroundingChunks[Chunk::West]   && m_surroundingChunks[Chunk::West]->isInitialized()
		&& m_surroundingChunks[Chunk::East]   && m_surroundingChunks[Chunk::East]->isInitialized()
		&& m_surroundingChunks[Chunk::South]  && m_surroundingChunks[Chunk::South]->isInitialized()
		&& m_surroundingChunks[Chunk::North]  && m_surroundingChunks[Chunk::North]->isInitialized()
		&& m_surroundingChunks[Chunk::Bottom] && m_surroundingChunks[Chunk::Bottom]->isInitialized()
		&& m_surroundingChunks[Chunk::Top]    && m_surroundingChunks[Chunk::Top]->isInitialized();
}

