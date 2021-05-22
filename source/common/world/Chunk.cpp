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
#include "World.hpp"

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

	u16 currentBlockID = m_data[z][y][x] & 0xffff;
	if (currentBlockID == type) return;

	u16 blockParam = getData(x, y, z);
	const Block &block = Registry::getInstance().getBlock(type);
	const BlockState &blockState = block.getState(block.param().hasParam(BlockParam::State)
		? block.param().getParam(BlockParam::State, blockParam) : 0);

	if (blockState.isLightSource())
		m_lightmap.addTorchlight(x, y, z, 14);
	else {
		m_lightmap.removeTorchlight(x, y, z);
		m_lightmap.removeSunlight(x, y, z);
	}

	onBlockPlaced(x, y, z, block);
	m_world.onBlockPlaced(x + m_x * width, y + m_y * depth, z + m_z * height, block);

	setBlockRaw(x, y, z, type);

	if (currentBlockID != 0) {
		const Block &oldBlock = Registry::getInstance().getBlock(currentBlockID);
		onBlockDestroyed(x, y, z, oldBlock);
	}
}

bool Chunk::setData(int x, int y, int z, u16 data) {
	if(x < 0)              return m_surroundingChunks[0] ? m_surroundingChunks[0]->setData(x + Chunk::width, y, z, data) : false;
	if(x >= Chunk::width)  return m_surroundingChunks[1] ? m_surroundingChunks[1]->setData(x - Chunk::width, y, z, data) : false;
	if(y < 0)              return m_surroundingChunks[2] ? m_surroundingChunks[2]->setData(x, y + Chunk::depth, z, data) : false;
	if(y >= Chunk::depth)  return m_surroundingChunks[3] ? m_surroundingChunks[3]->setData(x, y - Chunk::depth, z, data) : false;
	if(z < 0)              return m_surroundingChunks[4] ? m_surroundingChunks[4]->setData(x, y, z + Chunk::height, data) : false;
	if(z >= Chunk::height) return m_surroundingChunks[5] ? m_surroundingChunks[5]->setData(x, y, z - Chunk::height, data) : false;

	if (((m_data[z][y][x] >> 16) & 0xffff) == data) return false;

	m_data[z][y][x] &= 0xffff;
	m_data[z][y][x] |= (data << 16);

	m_hasChanged = true;
	m_world.addChunkToUpdate(this);

	return true;
}

bool Chunk::setBlockRaw(int x, int y, int z, u16 type) {
	if(x < 0)              return m_surroundingChunks[0] ? m_surroundingChunks[0]->setBlockRaw(x + Chunk::width, y, z, type) : false;
	if(x >= Chunk::width)  return m_surroundingChunks[1] ? m_surroundingChunks[1]->setBlockRaw(x - Chunk::width, y, z, type) : false;
	if(y < 0)              return m_surroundingChunks[2] ? m_surroundingChunks[2]->setBlockRaw(x, y + Chunk::depth, z, type) : false;
	if(y >= Chunk::depth)  return m_surroundingChunks[3] ? m_surroundingChunks[3]->setBlockRaw(x, y - Chunk::depth, z, type) : false;
	if(z < 0)              return m_surroundingChunks[4] ? m_surroundingChunks[4]->setBlockRaw(x, y, z + Chunk::height, type) : false;
	if(z >= Chunk::height) return m_surroundingChunks[5] ? m_surroundingChunks[5]->setBlockRaw(x, y, z - Chunk::height, type) : false;

	if ((m_data[z][y][x] & 0xffff) == type) return false;

	if (type == 0) {
		auto it = m_blockData.find(gk::Vector3i{x, y, z});
		if (it != m_blockData.end())
			m_blockData.erase(it);
	}

	m_data[z][y][x] &= 0xffff0000;
	m_data[z][y][x] |= type;

	m_hasChanged = true;
	m_world.addChunkToUpdate(this);

	return true;
}

const BlockState *Chunk::getBlockState(int x, int y, int z) const {
	if(x < 0)              return m_surroundingChunks[0] ? m_surroundingChunks[0]->getBlockState(x + Chunk::width, y, z) : nullptr;
	if(x >= Chunk::width)  return m_surroundingChunks[1] ? m_surroundingChunks[1]->getBlockState(x - Chunk::width, y, z) : nullptr;
	if(y < 0)              return m_surroundingChunks[2] ? m_surroundingChunks[2]->getBlockState(x, y + Chunk::depth, z) : nullptr;
	if(y >= Chunk::depth)  return m_surroundingChunks[3] ? m_surroundingChunks[3]->getBlockState(x, y - Chunk::depth, z) : nullptr;
	if(z < 0)              return m_surroundingChunks[4] ? m_surroundingChunks[4]->getBlockState(x, y, z + Chunk::height) : nullptr;
	if(z >= Chunk::height) return m_surroundingChunks[5] ? m_surroundingChunks[5]->getBlockState(x, y, z - Chunk::height) : nullptr;

	u16 blockID = getBlock(x, y, z);
	u16 blockParam = getData(x, y, z);
	const Block &block = Registry::getInstance().getBlock(blockID);
	if (!block.param().hasParam(BlockParam::State))
		return &block.getState(0);

	return &block.getState(block.param().getParam(BlockParam::State, blockParam));
}

void Chunk::setBlockState(int x, int y, int z, u16 stateID) {
	if(x < 0)              { if (m_surroundingChunks[0]) m_surroundingChunks[0]->setBlockState(x + Chunk::width, y, z, stateID); return; }
	if(x >= Chunk::width)  { if (m_surroundingChunks[1]) m_surroundingChunks[1]->setBlockState(x - Chunk::width, y, z, stateID); return; }
	if(y < 0)              { if (m_surroundingChunks[2]) m_surroundingChunks[2]->setBlockState(x, y + Chunk::depth, z, stateID); return; }
	if(y >= Chunk::depth)  { if (m_surroundingChunks[3]) m_surroundingChunks[3]->setBlockState(x, y - Chunk::depth, z, stateID); return; }
	if(z < 0)              { if (m_surroundingChunks[4]) m_surroundingChunks[4]->setBlockState(x, y, z + Chunk::height, stateID); return; }
	if(z >= Chunk::height) { if (m_surroundingChunks[5]) m_surroundingChunks[5]->setBlockState(x, y, z - Chunk::height, stateID); return; }

	u16 blockID = getBlock(x, y, z);
	u16 blockParam = getData(x, y, z);
	const Block &block = Registry::getInstance().getBlock(blockID);
	const BlockState &blockState = block.getState(block.param().hasParam(BlockParam::State)
		? block.param().getParam(BlockParam::State, blockParam) : 0);
	const BlockState &newBlockState = block.getState(block.param().hasParam(BlockParam::State)
		? stateID : 0);

	if (!blockState.isLightSource() && newBlockState.isLightSource()) {
		m_lightmap.addTorchlight(x, y, z, 14);
	}
	else if (blockState.isLightSource() && !newBlockState.isLightSource()) {
		gkDebug() << block.stringID();

		m_lightmap.removeTorchlight(x, y, z);
		m_lightmap.removeSunlight(x, y, z);
	}

	if (block.param().hasParam(BlockParam::State))
		setData(x, y, z, block.param().setParam(BlockParam::State, blockParam, stateID));
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

bool Chunk::areAllNeighboursInitialized() const {
	return m_surroundingChunks[Chunk::West]   && m_surroundingChunks[Chunk::West]->isInitialized()
		&& m_surroundingChunks[Chunk::East]   && m_surroundingChunks[Chunk::East]->isInitialized()
		&& m_surroundingChunks[Chunk::South]  && m_surroundingChunks[Chunk::South]->isInitialized()
		&& m_surroundingChunks[Chunk::North]  && m_surroundingChunks[Chunk::North]->isInitialized()
		&& m_surroundingChunks[Chunk::Bottom] && m_surroundingChunks[Chunk::Bottom]->isInitialized()
		&& m_surroundingChunks[Chunk::Top]    && m_surroundingChunks[Chunk::Top]->isInitialized();
}

// Please update 'docs/lua-api-cpp.md' if you change this
void Chunk::initUsertype(sol::state &lua) {
	lua.new_usertype<Chunk>("Chunk",
		"get_block", &Chunk::getBlock,
		"set_block", &Chunk::setBlock,

		"get_data", &Chunk::getData,
		"set_data", &Chunk::setData,

		"get_block_data", &Chunk::getBlockData,
		"add_block_data", &Chunk::addBlockData,

		"get_block_state", &Chunk::getBlockState,
		"set_block_state", &Chunk::setBlockState
	);
}

