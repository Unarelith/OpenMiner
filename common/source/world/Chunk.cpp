/*
 * =====================================================================================
 *
 *       Filename:  Chunk.cpp
 *
 *    Description:
 *
 *        Created:  15/12/2014 17:31:32
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include <cstring>

#include <gk/core/GameClock.hpp>

#include "Chunk.hpp"
#include "Config.hpp"
#include "Registry.hpp"

Chunk::Chunk(s32 x, s32 y, s32 z) {
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

// #include "Debug.hpp"

void Chunk::setBlock(int x, int y, int z, u16 type) {
	if(x < 0)              { if(m_surroundingChunks[0]) m_surroundingChunks[0]->setBlock(x + Chunk::width, y, z, type); return; }
	if(x >= Chunk::width)  { if(m_surroundingChunks[1]) m_surroundingChunks[1]->setBlock(x - Chunk::width, y, z, type); return; }
	if(y < 0)              { if(m_surroundingChunks[4]) m_surroundingChunks[4]->setBlock(x, y + Chunk::height, z, type); return; }
	if(y >= Chunk::height) { if(m_surroundingChunks[5]) m_surroundingChunks[5]->setBlock(x, y - Chunk::height, z, type); return; }
	if(z < 0)              { if(m_surroundingChunks[2]) m_surroundingChunks[2]->setBlock(x, y, z + Chunk::depth, type); return; }
	if(z >= Chunk::depth)  { if(m_surroundingChunks[3]) m_surroundingChunks[3]->setBlock(x, y, z - Chunk::depth, type); return; }

	// FIXME
	// const Block &block = Registry::getInstance().getBlock(type);
	// // if (type == 8)
	// // 	DEBUG("at (", m_x, m_y, m_z, ")", "(", x, y, z, ")", type, "is", block.canUpdate());
	// if (block.canUpdate()) {
	// 	m_tickingBlocks.emplace(x + y * width + z * width * height, block);
	// }
	// else {
	// 	auto it = m_tickingBlocks.find(x + y * width + z * width * height);
	// 	if (it != m_tickingBlocks.end())
	// 		m_tickingBlocks.erase(it);
	// }

	if (type == BlockType::Glowstone)
		m_lightmap.addLight(x, y, z, 14);
	else {
	// else if (m_data[x][y][z] == BlockType::Glowstone)
		m_lightmap.removeLight(x, y, z);
	// else {
		m_lightmap.removeSunlight(x, y, z);
	}

	// FIXME
	// if (type == BlockType::Workbench)
	// 	m_blockData.emplace(gk::Vector3i{x, y, z}, BlockData{3, 3});
	// else if (type == BlockType::Furnace)
	// 	m_blockData.emplace(gk::Vector3i{x, y, z}, BlockData{3, 1});
    //
	// if (m_data[x][y][z] == BlockType::Workbench || m_data[x][y][z] == BlockType::Furnace) {
	// 	auto it = m_blockData.find(gk::Vector3i{x, y, z});
	// 	if (it != m_blockData.end())
	// 		m_blockData.erase(it);
	// }

	m_data[x][y][z] = type;

	m_hasChanged = true;

	// FIXME
	// updateNeighbours(x, y, z);

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

	m_data[x][y][z] = type;

	m_hasChanged = true;
}

// FIXME
// BlockData *Chunk::getBlockData(int x, int y, int z) {
// 	if(x < 0)             return m_surroundingChunks[0] ? m_surroundingChunks[0]->getBlockData(x + CHUNK_WIDTH, y, z) : 0;
// 	if(x >= CHUNK_WIDTH)  return m_surroundingChunks[1] ? m_surroundingChunks[1]->getBlockData(x - CHUNK_WIDTH, y, z) : 0;
// 	if(y < 0)             return m_surroundingChunks[4] ? m_surroundingChunks[4]->getBlockData(x, y + CHUNK_HEIGHT, z) : 0;
// 	if(y >= CHUNK_HEIGHT) return m_surroundingChunks[5] ? m_surroundingChunks[5]->getBlockData(x, y - CHUNK_HEIGHT, z) : 0;
// 	if(z < 0)             return m_surroundingChunks[2] ? m_surroundingChunks[2]->getBlockData(x, y, z + CHUNK_DEPTH) : 0;
// 	if(z >= CHUNK_DEPTH)  return m_surroundingChunks[3] ? m_surroundingChunks[3]->getBlockData(x, y, z - CHUNK_DEPTH) : 0;
//
// 	auto it = m_blockData.find(gk::Vector3i{x, y, z});
// 	if (it == m_blockData.end())
// 		return nullptr;
//
// 	return &it->second;
// }

// FIXME
// void Chunk::updateNeighbours(int x, int y, int z) {
// 	int neighbours[7][3] = {
// 		{x, y, z},
// 		{x - 1, y, z},
// 		{x + 1, y, z},
// 		{x, y - 1, z},
// 		{x, y + 1, z},
// 		{x, y, z - 1},
// 		{x, y, z + 1},
// 	};
//
// 	for (u32 i = 0 ; i < 7 ; ++i) {
// 		u32 blockID = getBlock(neighbours[i][0], neighbours[i][1], neighbours[i][2]);
// 		if (blockID) {
// 			const Block &block = Registry::getInstance().getBlock(blockID);
// 			block.onNeighbourUpdate(glm::vec3{x, y, z},
// 			                        glm::vec3{neighbours[i][0], neighbours[i][1], neighbours[i][2]},
// 			                        *this);
// 		}
// 	}
// }

