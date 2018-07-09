/*
 * =====================================================================================
 *
 *       Filename:  ChunkLightmap.cpp
 *
 *    Description:
 *
 *        Created:  25/06/2018 14:32:44
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "Chunk.hpp"
#include "ChunkLightmap.hpp"
#include "Registry.hpp"

ChunkLightmap::ChunkLightmap(Chunk *chunk) : m_chunk(chunk) {
	std::memset(m_lightMap, 0, sizeof(m_lightMap));
}

void ChunkLightmap::addLight(int x, int y, int z, int val) {
	setTorchlight(x, y, z, val);
	m_lightBfsQueue.emplace(x, y, z);
}

void ChunkLightmap::addSunlight(int x, int y, int z, int val) {
	setSunlight(x, y, z, val);
	m_sunlightBfsQueue.emplace(x, y, z);
}

void ChunkLightmap::removeLight(int x, int y, int z) {
	m_lightRemovalBfsQueue.emplace(x, y, z, getTorchlight(x, y, z));
	setTorchlight(x, y, z, 0);
}

void ChunkLightmap::removeSunlight(int x, int y, int z) {
	m_sunlightRemovalBfsQueue.emplace(x, y, z, getSunlight(x, y, z));
	setSunlight(x, y, z, 0);
}

void ChunkLightmap::updateLights() {
	updateTorchlight();
	updateSunlight();
}

void ChunkLightmap::updateTorchlight() {
	while (!m_lightRemovalBfsQueue.empty()) {
		LightRemovalNode node = m_lightRemovalBfsQueue.front();
		m_lightRemovalBfsQueue.pop();

		LightNode surroundingNodes[6] = {
			{node.x - 1, node.y,     node.z},
			{node.x + 1, node.y,     node.z},
			{node.x,     node.y - 1, node.z},
			{node.x,     node.y + 1, node.z},
			{node.x,     node.y,     node.z - 1},
			{node.x,     node.y,     node.z + 1},
		};

		for (const LightNode &surroundingNode : surroundingNodes) {
			int level = getTorchlight(surroundingNode.x, surroundingNode.y, surroundingNode.z);
			if (level != 0 && level < node.value) {
				setTorchlight(surroundingNode.x, surroundingNode.y, surroundingNode.z, 0);

				m_lightRemovalBfsQueue.emplace(surroundingNode.x, surroundingNode.y, surroundingNode.z, level);
			}
			else if (level >= node.value) {
				m_lightBfsQueue.emplace(surroundingNode.x, surroundingNode.y, surroundingNode.z);
			}
		}
	}
	while (!m_lightBfsQueue.empty()) {
		LightNode node = m_lightBfsQueue.front();
		m_lightBfsQueue.pop();

		LightNode surroundingNodes[6] = {
			{node.x - 1, node.y,     node.z},
			{node.x + 1, node.y,     node.z},
			{node.x,     node.y - 1, node.z},
			{node.x,     node.y + 1, node.z},
			{node.x,     node.y,     node.z - 1},
			{node.x,     node.y,     node.z + 1},
		};

		int lightLevel = getTorchlight(node.x, node.y, node.z);
		for (const LightNode &surroundingNode : surroundingNodes) {
			if (getTorchlight(surroundingNode.x, surroundingNode.y, surroundingNode.z) + 2 <= lightLevel) {
				setTorchlight(surroundingNode.x, surroundingNode.y, surroundingNode.z, lightLevel - 1);

				if (!Registry::getInstance().getBlock(m_chunk->getBlock(surroundingNode.x, surroundingNode.y, surroundingNode.z)).isOpaque()) {
					m_lightBfsQueue.emplace(surroundingNode.x, surroundingNode.y, surroundingNode.z);
				}
			}
		}
	}
}

void ChunkLightmap::updateSunlight() {
	while (!m_sunlightRemovalBfsQueue.empty()) {
		LightRemovalNode node = m_sunlightRemovalBfsQueue.front();
		m_sunlightRemovalBfsQueue.pop();

		LightNode surroundingNodes[6] = {
			{node.x - 1, node.y,     node.z},
			{node.x + 1, node.y,     node.z},
			{node.x,     node.y - 1, node.z},
			{node.x,     node.y + 1, node.z},
			{node.x,     node.y,     node.z - 1},
			{node.x,     node.y,     node.z + 1},
		};

		for (const LightNode &surroundingNode : surroundingNodes) {
			int level = getSunlight(surroundingNode.x, surroundingNode.y, surroundingNode.z);
			if (level != 0 && level < node.value) {
				setSunlight(surroundingNode.x, surroundingNode.y, surroundingNode.z, 0);

				m_sunlightRemovalBfsQueue.emplace(surroundingNode.x, surroundingNode.y, surroundingNode.z, level);
			}
			else if (level >= node.value) {
				m_sunlightBfsQueue.emplace(surroundingNode.x, surroundingNode.y, surroundingNode.z);
			}
		}
	}
	while (!m_sunlightBfsQueue.empty()) {
		LightNode node = m_sunlightBfsQueue.front();
		m_sunlightBfsQueue.pop();

		LightNode surroundingNodes[6] = {
			{node.x - 1, node.y,     node.z},
			{node.x + 1, node.y,     node.z},
			{node.x,     node.y - 1, node.z},
			{node.x,     node.y + 1, node.z},
			{node.x,     node.y,     node.z - 1},
			{node.x,     node.y,     node.z + 1},
		};

		int sunlightLevel = getSunlight(node.x, node.y, node.z);
		for (const LightNode &surroundingNode : surroundingNodes) {
			if (getSunlight(surroundingNode.x, surroundingNode.y, surroundingNode.z) + 2 <= sunlightLevel) {
				// if (sunlightLevel == 15 && surroundingNode.y == node.y - 1)
				// 	setSunlight(surroundingNode.x, surroundingNode.y, surroundingNode.z, sunlightLevel);
				// else
					setSunlight(surroundingNode.x, surroundingNode.y, surroundingNode.z, sunlightLevel - 1);

				m_sunlightBfsQueue.emplace(surroundingNode.x, surroundingNode.y, surroundingNode.z);
			}
		}
	}
}

int ChunkLightmap::getSunlight(int x, int y, int z) const {
	if(x < 0)              return m_chunk->getSurroundingChunk(0) ? m_chunk->getSurroundingChunk(0)->lightmap().getSunlight(x + Chunk::width, y, z) : 0;
	if(x >= Chunk::width)  return m_chunk->getSurroundingChunk(1) ? m_chunk->getSurroundingChunk(1)->lightmap().getSunlight(x - Chunk::width, y, z) : 0;
	if(y < 0)              return m_chunk->getSurroundingChunk(4) ? m_chunk->getSurroundingChunk(4)->lightmap().getSunlight(x, y + Chunk::height, z) : 0;
	if(y >= Chunk::height) return m_chunk->getSurroundingChunk(5) ? m_chunk->getSurroundingChunk(5)->lightmap().getSunlight(x, y - Chunk::height, z) : 0;
	if(z < 0)              return m_chunk->getSurroundingChunk(2) ? m_chunk->getSurroundingChunk(2)->lightmap().getSunlight(x, y, z + Chunk::depth) : 0;
	if(z >= Chunk::depth)  return m_chunk->getSurroundingChunk(3) ? m_chunk->getSurroundingChunk(3)->lightmap().getSunlight(x, y, z - Chunk::depth) : 0;

	return (m_lightMap[x][y][z] >> 4) & 0xf;
}

int ChunkLightmap::getTorchlight(int x, int y, int z) const {
	if(x < 0)              return m_chunk->getSurroundingChunk(0) ? m_chunk->getSurroundingChunk(0)->lightmap().getTorchlight(x + Chunk::width, y, z) : 0;
	if(x >= Chunk::width)  return m_chunk->getSurroundingChunk(1) ? m_chunk->getSurroundingChunk(1)->lightmap().getTorchlight(x - Chunk::width, y, z) : 0;
	if(y < 0)              return m_chunk->getSurroundingChunk(4) ? m_chunk->getSurroundingChunk(4)->lightmap().getTorchlight(x, y + Chunk::height, z) : 0;
	if(y >= Chunk::height) return m_chunk->getSurroundingChunk(5) ? m_chunk->getSurroundingChunk(5)->lightmap().getTorchlight(x, y - Chunk::height, z) : 0;
	if(z < 0)              return m_chunk->getSurroundingChunk(2) ? m_chunk->getSurroundingChunk(2)->lightmap().getTorchlight(x, y, z + Chunk::depth) : 0;
	if(z >= Chunk::depth)  return m_chunk->getSurroundingChunk(3) ? m_chunk->getSurroundingChunk(3)->lightmap().getTorchlight(x, y, z - Chunk::depth) : 0;

	return m_lightMap[x][y][z] & 0xf;
}

void ChunkLightmap::setSunlight(int x, int y, int z, int val) {
	if(x < 0)              { if(m_chunk->getSurroundingChunk(0)) m_chunk->getSurroundingChunk(0)->lightmap().setSunlight(x + Chunk::width, y, z, val); return; }
	if(x >= Chunk::width)  { if(m_chunk->getSurroundingChunk(1)) m_chunk->getSurroundingChunk(1)->lightmap().setSunlight(x - Chunk::width, y, z, val); return; }
	if(y < 0)              { if(m_chunk->getSurroundingChunk(4)) m_chunk->getSurroundingChunk(4)->lightmap().setSunlight(x, y + Chunk::height, z, val); return; }
	if(y >= Chunk::height) { if(m_chunk->getSurroundingChunk(5)) m_chunk->getSurroundingChunk(5)->lightmap().setSunlight(x, y - Chunk::height, z, val); return; }
	if(z < 0)              { if(m_chunk->getSurroundingChunk(2)) m_chunk->getSurroundingChunk(2)->lightmap().setSunlight(x, y, z + Chunk::depth, val); return; }
	if(z >= Chunk::depth)  { if(m_chunk->getSurroundingChunk(3)) m_chunk->getSurroundingChunk(3)->lightmap().setSunlight(x, y, z - Chunk::depth, val); return; }

	m_lightMap[x][y][z] = (m_lightMap[x][y][z] & 0xf) | (val << 4);

	m_chunk->setChanged(true);

	if(x == 0                 && m_chunk->getSurroundingChunk(Chunk::Left))   { m_chunk->getSurroundingChunk(Chunk::Left)->setChanged(true); }
	if(x == Chunk::width - 1  && m_chunk->getSurroundingChunk(Chunk::Right))  { m_chunk->getSurroundingChunk(Chunk::Right)->setChanged(true); }
	if(y == 0                 && m_chunk->getSurroundingChunk(Chunk::Bottom)) { m_chunk->getSurroundingChunk(Chunk::Bottom)->setChanged(true); }
	if(y == Chunk::height - 1 && m_chunk->getSurroundingChunk(Chunk::Top))    { m_chunk->getSurroundingChunk(Chunk::Top)->setChanged(true); }
	if(z == 0                 && m_chunk->getSurroundingChunk(Chunk::Front))  { m_chunk->getSurroundingChunk(Chunk::Front)->setChanged(true); }
	if(z == Chunk::depth - 1  && m_chunk->getSurroundingChunk(Chunk::Back))   { m_chunk->getSurroundingChunk(Chunk::Back)->setChanged(true); }
};

void ChunkLightmap::setTorchlight(int x, int y, int z, int val) {
	if(x < 0)              { if(m_chunk->getSurroundingChunk(0)) m_chunk->getSurroundingChunk(0)->lightmap().setTorchlight(x + Chunk::width, y, z, val); return; }
	if(x >= Chunk::width)  { if(m_chunk->getSurroundingChunk(1)) m_chunk->getSurroundingChunk(1)->lightmap().setTorchlight(x - Chunk::width, y, z, val); return; }
	if(y < 0)              { if(m_chunk->getSurroundingChunk(4)) m_chunk->getSurroundingChunk(4)->lightmap().setTorchlight(x, y + Chunk::height, z, val); return; }
	if(y >= Chunk::height) { if(m_chunk->getSurroundingChunk(5)) m_chunk->getSurroundingChunk(5)->lightmap().setTorchlight(x, y - Chunk::height, z, val); return; }
	if(z < 0)              { if(m_chunk->getSurroundingChunk(2)) m_chunk->getSurroundingChunk(2)->lightmap().setTorchlight(x, y, z + Chunk::depth, val); return; }
	if(z >= Chunk::depth)  { if(m_chunk->getSurroundingChunk(3)) m_chunk->getSurroundingChunk(3)->lightmap().setTorchlight(x, y, z - Chunk::depth, val); return; }

	m_lightMap[x][y][z] = (m_lightMap[x][y][z] & 0xf0) | val;

	m_chunk->setChanged(true);

	if(x == 0                 && m_chunk->getSurroundingChunk(Chunk::Left))   { m_chunk->getSurroundingChunk(Chunk::Left)->setChanged(true); }
	if(x == Chunk::width - 1  && m_chunk->getSurroundingChunk(Chunk::Right))  { m_chunk->getSurroundingChunk(Chunk::Right)->setChanged(true); }
	if(y == 0                 && m_chunk->getSurroundingChunk(Chunk::Bottom)) { m_chunk->getSurroundingChunk(Chunk::Bottom)->setChanged(true); }
	if(y == Chunk::height - 1 && m_chunk->getSurroundingChunk(Chunk::Top))    { m_chunk->getSurroundingChunk(Chunk::Top)->setChanged(true); }
	if(z == 0                 && m_chunk->getSurroundingChunk(Chunk::Front))  { m_chunk->getSurroundingChunk(Chunk::Front)->setChanged(true); }
	if(z == Chunk::depth - 1  && m_chunk->getSurroundingChunk(Chunk::Back))   { m_chunk->getSurroundingChunk(Chunk::Back)->setChanged(true); }
}

