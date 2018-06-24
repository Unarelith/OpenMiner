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
#include "Chunk.hpp"
#include "Registry.hpp"

Chunk::Chunk(s32 x, s32 y, s32 z, Texture &texture) : m_texture(texture) {
	m_x = x;
	m_y = y;
	m_z = z;

	std::memset(m_data, 0, sizeof(m_data));
	std::memset(m_lightMap, 0, sizeof(m_lightMap));
}

void Chunk::update(World &world) {
	if (!m_isChanged) return;

	m_isChanged = false;

	m_verticesCount = m_builder.buildChunk(*this, m_vbo);

	// for (auto &it : m_tickingBlocks) {
	// 	int z = it.first / (width * height);
	// 	int y = (it.first - z * width * height) / width;
	// 	int x = (it.first - z * width * height) % width;
	// 	it.second.onTick(glm::ivec3{x + m_x * width, y + m_y * height, z + m_z * depth}, world);
	// }
}

u32 Chunk::getBlock(int x, int y, int z) const {
	if(x < 0)              return m_surroundingChunks[0] ? m_surroundingChunks[0]->getBlock(x + Chunk::width, y, z) : 0;
	if(x >= Chunk::width)  return m_surroundingChunks[1] ? m_surroundingChunks[1]->getBlock(x - Chunk::width, y, z) : 0;
	if(y < 0)              return m_surroundingChunks[4] ? m_surroundingChunks[4]->getBlock(x, y + Chunk::height, z) : 0;
	if(y >= Chunk::height) return m_surroundingChunks[5] ? m_surroundingChunks[5]->getBlock(x, y - Chunk::height, z) : 0;
	if(z < 0)              return m_surroundingChunks[2] ? m_surroundingChunks[2]->getBlock(x, y, z + Chunk::depth) : 0;
	if(z >= Chunk::depth)  return m_surroundingChunks[3] ? m_surroundingChunks[3]->getBlock(x, y, z - Chunk::depth) : 0;
	return m_data[x][y][z];
}

// #include "Debug.hpp"

void Chunk::setBlock(int x, int y, int z, u32 type) {
	if(x < 0)              { if(m_surroundingChunks[0]) m_surroundingChunks[0]->setBlock(x + Chunk::width, y, z, type); return; }
	if(x >= Chunk::width)  { if(m_surroundingChunks[1]) m_surroundingChunks[1]->setBlock(x - Chunk::width, y, z, type); return; }
	if(y < 0)              { if(m_surroundingChunks[4]) m_surroundingChunks[4]->setBlock(x, y + Chunk::height, z, type); return; }
	if(y >= Chunk::height) { if(m_surroundingChunks[5]) m_surroundingChunks[5]->setBlock(x, y - Chunk::height, z, type); return; }
	if(z < 0)              { if(m_surroundingChunks[2]) m_surroundingChunks[2]->setBlock(x, y, z + Chunk::depth, type); return; }
	if(z >= Chunk::depth)  { if(m_surroundingChunks[3]) m_surroundingChunks[3]->setBlock(x, y, z - Chunk::depth, type); return; }

	const Block &block = Registry::getInstance().getBlock(type);
	// if (type == 8)
	// 	DEBUG("at (", m_x, m_y, m_z, ")", "(", x, y, z, ")", type, "is", block.canUpdate());
	if (block.canUpdate()) {
		m_tickingBlocks.emplace(x + y * width + z * width * height, block);
	}
	else {
		auto it = m_tickingBlocks.find(x + y * width + z * width * height);
		if (it != m_tickingBlocks.end())
			m_tickingBlocks.erase(it);
	}

	if (type == 2)
		addLight(*this, x, y, z, 14);
	else if (m_data[x][y][z] == 2)
		removeLight(*this, x, y, z);

	m_data[x][y][z] = type;

	m_isChanged = true;

	if(x == 0          && left())  { left()->m_isChanged = true; }
	if(x == width - 1  && right()) { right()->m_isChanged = true; }
	if(y == 0          && below()) { below()->m_isChanged = true; }
	if(y == height - 1 && above()) { above()->m_isChanged = true; }
	if(z == 0          && front()) { front()->m_isChanged = true; }
	if(z == depth - 1  && back())  { back()->m_isChanged = true; }
}

void Chunk::addLight(Chunk &chunk, int x, int y, int z, int val) {
	chunk.setTorchlight(x, y, z, val);
	m_lightBfsQueue.emplace(x, y, z);
	updateLights(chunk);
}

void Chunk::removeLight(Chunk &chunk, int x, int y, int z) {
	m_lightRemovalBfsQueue.emplace(x, y, z, chunk.getTorchlight(x, y, z));
	chunk.setTorchlight(x, y, z, 0);
	updateLights(chunk);
}

void Chunk::updateLights(Chunk &chunk) {
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
			int level = chunk.getTorchlight(surroundingNode.x, surroundingNode.y, surroundingNode.z);
			if (level != 0 && level < node.value) {
				chunk.setTorchlight(surroundingNode.x, surroundingNode.y, surroundingNode.z, 0);

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

		int lightLevel = chunk.getTorchlight(node.x, node.y, node.z);
		for (const LightNode &surroundingNode : surroundingNodes) {
			if (/*!Registry::getInstance().getBlock(getBlock(surroundingNode.x, surroundingNode.y, surroundingNode.z)).isOpaque()
			&& */ chunk.getTorchlight(surroundingNode.x, surroundingNode.y, surroundingNode.z) + 2 <= lightLevel) {
				chunk.setTorchlight(surroundingNode.x, surroundingNode.y, surroundingNode.z, lightLevel - 1);

				m_lightBfsQueue.emplace(surroundingNode.x, surroundingNode.y, surroundingNode.z);
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

		int sunlightLevel = chunk.getSunlight(node.x, node.y, node.z);
		for (const LightNode &surroundingNode : surroundingNodes) {
			if (/*!Registry::getInstance().getBlock(getBlock(surroundingNode.x, surroundingNode.y, surroundingNode.z)).isOpaque()
			&& */ chunk.getSunlight(surroundingNode.x, surroundingNode.y, surroundingNode.z) + 2 <= sunlightLevel) {
				chunk.setSunlight(surroundingNode.x, surroundingNode.y, surroundingNode.z, sunlightLevel - 1);

				m_sunlightBfsQueue.emplace(surroundingNode.x, surroundingNode.y, surroundingNode.z);
			}
		}
	}
}

int Chunk::getSunlight(int x, int y, int z) const {
	if(x < 0)              return m_surroundingChunks[0] ? m_surroundingChunks[0]->getSunlight(x + Chunk::width, y, z) : 0;
	if(x >= Chunk::width)  return m_surroundingChunks[1] ? m_surroundingChunks[1]->getSunlight(x - Chunk::width, y, z) : 0;
	if(y < 0)              return m_surroundingChunks[4] ? m_surroundingChunks[4]->getSunlight(x, y + Chunk::height, z) : 0;
	if(y >= Chunk::height) return m_surroundingChunks[5] ? m_surroundingChunks[5]->getSunlight(x, y - Chunk::height, z) : 0;
	if(z < 0)              return m_surroundingChunks[2] ? m_surroundingChunks[2]->getSunlight(x, y, z + Chunk::depth) : 0;
	if(z >= Chunk::depth)  return m_surroundingChunks[3] ? m_surroundingChunks[3]->getSunlight(x, y, z - Chunk::depth) : 0;
	return (m_lightMap[x][y][z] >> 4) & 0xf;
}

void Chunk::setSunlight(int x, int y, int z, int val) {
	if(x < 0)              { if(m_surroundingChunks[0]) m_surroundingChunks[0]->setSunlight(x + Chunk::width, y, z, val); return; }
	if(x >= Chunk::width)  { if(m_surroundingChunks[1]) m_surroundingChunks[1]->setSunlight(x - Chunk::width, y, z, val); return; }
	if(y < 0)              { if(m_surroundingChunks[4]) m_surroundingChunks[4]->setSunlight(x, y + Chunk::height, z, val); return; }
	if(y >= Chunk::height) { if(m_surroundingChunks[5]) m_surroundingChunks[5]->setSunlight(x, y - Chunk::height, z, val); return; }
	if(z < 0)              { if(m_surroundingChunks[2]) m_surroundingChunks[2]->setSunlight(x, y, z + Chunk::depth, val); return; }
	if(z >= Chunk::depth)  { if(m_surroundingChunks[3]) m_surroundingChunks[3]->setSunlight(x, y, z - Chunk::depth, val); return; }
	m_lightMap[x][y][z] = (m_lightMap[x][y][z] & 0xf) | (val << 4);

	m_isChanged = true;
}

int Chunk::getTorchlight(int x, int y, int z) const {
	if(x < 0)              return m_surroundingChunks[0] ? m_surroundingChunks[0]->getTorchlight(x + Chunk::width, y, z) : 0;
	if(x >= Chunk::width)  return m_surroundingChunks[1] ? m_surroundingChunks[1]->getTorchlight(x - Chunk::width, y, z) : 0;
	if(y < 0)              return m_surroundingChunks[4] ? m_surroundingChunks[4]->getTorchlight(x, y + Chunk::height, z) : 0;
	if(y >= Chunk::height) return m_surroundingChunks[5] ? m_surroundingChunks[5]->getTorchlight(x, y - Chunk::height, z) : 0;
	if(z < 0)              return m_surroundingChunks[2] ? m_surroundingChunks[2]->getTorchlight(x, y, z + Chunk::depth) : 0;
	if(z >= Chunk::depth)  return m_surroundingChunks[3] ? m_surroundingChunks[3]->getTorchlight(x, y, z - Chunk::depth) : 0;
	return m_lightMap[x][y][z] & 0xf;
}

void Chunk::setTorchlight(int x, int y, int z, int val) {
	if(x < 0)              { if(m_surroundingChunks[0]) m_surroundingChunks[0]->setTorchlight(x + Chunk::width, y, z, val); return; }
	if(x >= Chunk::width)  { if(m_surroundingChunks[1]) m_surroundingChunks[1]->setTorchlight(x - Chunk::width, y, z, val); return; }
	if(y < 0)              { if(m_surroundingChunks[4]) m_surroundingChunks[4]->setTorchlight(x, y + Chunk::height, z, val); return; }
	if(y >= Chunk::height) { if(m_surroundingChunks[5]) m_surroundingChunks[5]->setTorchlight(x, y - Chunk::height, z, val); return; }
	if(z < 0)              { if(m_surroundingChunks[2]) m_surroundingChunks[2]->setTorchlight(x, y, z + Chunk::depth, val); return; }
	if(z >= Chunk::depth)  { if(m_surroundingChunks[3]) m_surroundingChunks[3]->setTorchlight(x, y, z - Chunk::depth, val); return; }
	m_lightMap[x][y][z] = (m_lightMap[x][y][z] & 0xf0) | val;

	m_isChanged = true;
}


void Chunk::draw(RenderTarget &target, RenderStates states) const {
	if(m_verticesCount == 0) return;

	states.texture = &m_texture;

	target.draw(m_vbo, GL_QUADS, 0, m_verticesCount, states);

	// drawOutlines(target, states);
}

void Chunk::drawOutlines(RenderTarget &target, RenderStates states) const {
	states.texture = nullptr;

	for(u32 i = 0 ; i < m_verticesCount ; i += 4) {
		target.draw(m_vbo, GL_LINE_LOOP, i, 4, states);
	}
}

