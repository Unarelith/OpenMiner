/*
 * =====================================================================================
 *
 *  OpenMiner
 *  Copyright (C) 2018-2020 Unarelith, Quentin Bazin <openminer@unarelith.net>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program; if not, write to the Free Software Foundation, Inc.,
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
	if(y < 0)              return m_surroundingChunks[4] ? m_surroundingChunks[4]->getBlock(x, y + Chunk::height, z) : 0;
	if(y >= Chunk::height) return m_surroundingChunks[5] ? m_surroundingChunks[5]->getBlock(x, y - Chunk::height, z) : 0;
	if(z < 0)              return m_surroundingChunks[2] ? m_surroundingChunks[2]->getBlock(x, y, z + Chunk::depth) : 0;
	if(z >= Chunk::depth)  return m_surroundingChunks[3] ? m_surroundingChunks[3]->getBlock(x, y, z - Chunk::depth) : 0;
	return m_data[x][y][z] & 0xffff;
}

u16 Chunk::getData(int x, int y, int z) const {
	if(x < 0)              return m_surroundingChunks[0] ? m_surroundingChunks[0]->getData(x + Chunk::width, y, z) : 0;
	if(x >= Chunk::width)  return m_surroundingChunks[1] ? m_surroundingChunks[1]->getData(x - Chunk::width, y, z) : 0;
	if(y < 0)              return m_surroundingChunks[4] ? m_surroundingChunks[4]->getData(x, y + Chunk::height, z) : 0;
	if(y >= Chunk::height) return m_surroundingChunks[5] ? m_surroundingChunks[5]->getData(x, y - Chunk::height, z) : 0;
	if(z < 0)              return m_surroundingChunks[2] ? m_surroundingChunks[2]->getData(x, y, z + Chunk::depth) : 0;
	if(z >= Chunk::depth)  return m_surroundingChunks[3] ? m_surroundingChunks[3]->getData(x, y, z - Chunk::depth) : 0;
	return (m_data[x][y][z] >> 16) & 0xffff;
}

void Chunk::setBlock(int x, int y, int z, u16 type) {
	if(x < 0)              { if(m_surroundingChunks[0]) m_surroundingChunks[0]->setBlock(x + Chunk::width, y, z, type); return; }
	if(x >= Chunk::width)  { if(m_surroundingChunks[1]) m_surroundingChunks[1]->setBlock(x - Chunk::width, y, z, type); return; }
	if(y < 0)              { if(m_surroundingChunks[4]) m_surroundingChunks[4]->setBlock(x, y + Chunk::height, z, type); return; }
	if(y >= Chunk::height) { if(m_surroundingChunks[5]) m_surroundingChunks[5]->setBlock(x, y - Chunk::height, z, type); return; }
	if(z < 0)              { if(m_surroundingChunks[2]) m_surroundingChunks[2]->setBlock(x, y, z + Chunk::depth, type); return; }
	if(z >= Chunk::depth)  { if(m_surroundingChunks[3]) m_surroundingChunks[3]->setBlock(x, y, z - Chunk::depth, type); return; }

	if (m_data[x][y][z] == type) return;

	const Block &block = Registry::getInstance().getBlock(type);
	if (block.canUpdate()) {
		m_tickingBlocks.emplace(x + y * width + z * width * height, block);
	}
	else {
		auto it = m_tickingBlocks.find(x + y * width + z * width * height);
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

	if(x == 0          && m_surroundingChunks[Left])   { m_surroundingChunks[Left]->m_hasChanged = true; }
	if(x == width - 1  && m_surroundingChunks[Right])  { m_surroundingChunks[Right]->m_hasChanged = true; }
	if(y == 0          && m_surroundingChunks[Bottom]) { m_surroundingChunks[Bottom]->m_hasChanged = true; }
	if(y == height - 1 && m_surroundingChunks[Top])    { m_surroundingChunks[Top]->m_hasChanged = true; }
	if(z == 0          && m_surroundingChunks[Front])  { m_surroundingChunks[Front]->m_hasChanged = true; }
	if(z == depth - 1  && m_surroundingChunks[Back])   { m_surroundingChunks[Back]->m_hasChanged = true; }
}

void Chunk::setData(int x, int y, int z, u16 data) {
	if(x < 0)              { if(m_surroundingChunks[0]) m_surroundingChunks[0]->setData(x + Chunk::width, y, z, data); return; }
	if(x >= Chunk::width)  { if(m_surroundingChunks[1]) m_surroundingChunks[1]->setData(x - Chunk::width, y, z, data); return; }
	if(y < 0)              { if(m_surroundingChunks[4]) m_surroundingChunks[4]->setData(x, y + Chunk::height, z, data); return; }
	if(y >= Chunk::height) { if(m_surroundingChunks[5]) m_surroundingChunks[5]->setData(x, y - Chunk::height, z, data); return; }
	if(z < 0)              { if(m_surroundingChunks[2]) m_surroundingChunks[2]->setData(x, y, z + Chunk::depth, data); return; }
	if(z >= Chunk::depth)  { if(m_surroundingChunks[3]) m_surroundingChunks[3]->setData(x, y, z - Chunk::depth, data); return; }

	m_data[x][y][z] &= 0xffff;
	m_data[x][y][z] |= (data << 16);

	m_hasChanged = true;
}

void Chunk::setBlockRaw(int x, int y, int z, u16 type) {
	if(x < 0)              { if(m_surroundingChunks[0]) m_surroundingChunks[0]->setBlockRaw(x + Chunk::width, y, z, type); return; }
	if(x >= Chunk::width)  { if(m_surroundingChunks[1]) m_surroundingChunks[1]->setBlockRaw(x - Chunk::width, y, z, type); return; }
	if(y < 0)              { if(m_surroundingChunks[4]) m_surroundingChunks[4]->setBlockRaw(x, y + Chunk::height, z, type); return; }
	if(y >= Chunk::height) { if(m_surroundingChunks[5]) m_surroundingChunks[5]->setBlockRaw(x, y - Chunk::height, z, type); return; }
	if(z < 0)              { if(m_surroundingChunks[2]) m_surroundingChunks[2]->setBlockRaw(x, y, z + Chunk::depth, type); return; }
	if(z >= Chunk::depth)  { if(m_surroundingChunks[3]) m_surroundingChunks[3]->setBlockRaw(x, y, z - Chunk::depth, type); return; }

	if (m_data[x][y][z] == type) return;

	if (type == 0) {
		auto it = m_blockData.find(gk::Vector3i{x, y, z});
		if (it != m_blockData.end())
			m_blockData.erase(it);
	}

	m_data[x][y][z] = type;

	m_hasChanged = true;
}

BlockData *Chunk::getBlockData(int x, int y, int z) const {
	if(x < 0)             return m_surroundingChunks[0] ? m_surroundingChunks[0]->getBlockData(x + CHUNK_WIDTH, y, z) : 0;
	if(x >= CHUNK_WIDTH)  return m_surroundingChunks[1] ? m_surroundingChunks[1]->getBlockData(x - CHUNK_WIDTH, y, z) : 0;
	if(y < 0)             return m_surroundingChunks[4] ? m_surroundingChunks[4]->getBlockData(x, y + CHUNK_HEIGHT, z) : 0;
	if(y >= CHUNK_HEIGHT) return m_surroundingChunks[5] ? m_surroundingChunks[5]->getBlockData(x, y - CHUNK_HEIGHT, z) : 0;
	if(z < 0)             return m_surroundingChunks[2] ? m_surroundingChunks[2]->getBlockData(x, y, z + CHUNK_DEPTH) : 0;
	if(z >= CHUNK_DEPTH)  return m_surroundingChunks[3] ? m_surroundingChunks[3]->getBlockData(x, y, z - CHUNK_DEPTH) : 0;

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
	if(y < 0)             return m_surroundingChunks[4] ? m_surroundingChunks[4]->addBlockData(x, y + CHUNK_HEIGHT, z) : 0;
	if(y >= CHUNK_HEIGHT) return m_surroundingChunks[5] ? m_surroundingChunks[5]->addBlockData(x, y - CHUNK_HEIGHT, z) : 0;
	if(z < 0)             return m_surroundingChunks[2] ? m_surroundingChunks[2]->addBlockData(x, y, z + CHUNK_DEPTH) : 0;
	if(z >= CHUNK_DEPTH)  return m_surroundingChunks[3] ? m_surroundingChunks[3]->addBlockData(x, y, z - CHUNK_DEPTH) : 0;

	gk::Vector3i pos{x, y, z};
	auto it = m_blockData.find(pos);
	if (it == m_blockData.end()) {
		gk::Vector3i absolutePos{x + m_x * CHUNK_WIDTH, y + m_y * CHUNK_HEIGHT, z + m_z * CHUNK_DEPTH};
		BlockData *data = new BlockData{absolutePos, inventoryWidth, inventoryHeight};
		m_blockData.emplace(pos, data);
		return data;
	}

	return it->second.get();
}

bool Chunk::areAllNeighboursLoaded() const {
	return m_surroundingChunks[Chunk::Left]
		&& m_surroundingChunks[Chunk::Right]
		&& m_surroundingChunks[Chunk::Front]
		&& m_surroundingChunks[Chunk::Back]
		&& m_surroundingChunks[Chunk::Bottom]
		&& m_surroundingChunks[Chunk::Top];
}

bool Chunk::areAllNeighboursInitialized() const {
	return m_surroundingChunks[Chunk::Left]   && m_surroundingChunks[Chunk::Left]->isInitialized()
		&& m_surroundingChunks[Chunk::Right]  && m_surroundingChunks[Chunk::Right]->isInitialized()
		&& m_surroundingChunks[Chunk::Front]  && m_surroundingChunks[Chunk::Front]->isInitialized()
		&& m_surroundingChunks[Chunk::Back]   && m_surroundingChunks[Chunk::Back]->isInitialized()
		&& m_surroundingChunks[Chunk::Bottom] && m_surroundingChunks[Chunk::Bottom]->isInitialized()
		&& m_surroundingChunks[Chunk::Top]    && m_surroundingChunks[Chunk::Top]->isInitialized();
}

