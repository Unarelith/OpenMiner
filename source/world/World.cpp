/*
 * =====================================================================================
 *
 *       Filename:  World.cpp
 *
 *    Description:
 *
 *        Created:  16/12/2014 15:28:19
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#include <gk/resource/ResourceHandler.hpp>

#include "Config.hpp"
#include "World.hpp"

u16 World::renderDistance = 8;
bool World::isReloadRequested = false;

World::World() : m_texture(gk::ResourceHandler::getInstance().get<gk::Texture>("texture-blocks")) {
	for(s32 z = 0 ; z < m_depth ; z++) {
		for(s32 y = 0 ; y < m_height ; y++) {
			for(s32 x = 0 ; x < m_width ; x++) {
				m_chunks.push_back(std::unique_ptr<Chunk>(new Chunk(x - m_width / 2,
				                                                    y - m_height / 2,
				                                                    z - m_depth / 2, m_texture)));
			}
		}
	}

	for(s32 z = -m_depth / 2 ; z < m_depth / 2 ; z++) {
		for(s32 y = -m_height / 2 ; y < m_height / 2 ; y++) {
			for(s32 x = -m_width / 2 ; x < m_width / 2 ; x++) {
				Chunk *chunk = getChunk(x, y, z);
				if(x > -m_width / 2)      chunk->setSurroundingChunk(Chunk::Left,   getChunk(x - 1, y, z));
				if(x <  m_width / 2 - 1)  chunk->setSurroundingChunk(Chunk::Right,  getChunk(x + 1, y, z));
				if(y > -m_height / 2)     chunk->setSurroundingChunk(Chunk::Bottom, getChunk(x, y - 1, z));
				if(y <  m_height / 2 - 1) chunk->setSurroundingChunk(Chunk::Top,    getChunk(x, y + 1, z));
				if(z > -m_depth / 2)      chunk->setSurroundingChunk(Chunk::Front,  getChunk(x, y, z - 1));
				if(z <  m_depth / 2 - 1)  chunk->setSurroundingChunk(Chunk::Back,   getChunk(x, y, z + 1));
			}
		}
	}
}

void World::update(Player &player) {
	for (auto &it : m_chunks) {
		if (isReloadRequested)
			it->setChanged(true);

		it->update(player, *this);
	}

	isReloadRequested = false;
}

void World::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	float ud = 1000.0;
	int ux = 0;
	int uy = 0;
	int uz = 0;

	gk::Shader::bind(states.shader);
	states.shader->setUniform("u_renderDistance", renderDistance * Chunk::width);
	gk::Shader::bind(nullptr);

	std::vector<std::pair<Chunk*, gk::Transform>> chunks;
	for(auto &it : m_chunks) {
		states.transform = glm::translate(glm::mat4(1.0f),
		                                  glm::vec3(it->x() * Chunk::width,
		                                            it->y() * Chunk::height,
		                                            it->z() * Chunk::depth));

		// Is the chunk close enough?
		glm::vec4 center = states.viewMatrix.getMatrix()
		                 * states.transform.getMatrix()
		                 * glm::vec4(Chunk::width / 2, Chunk::height / 2, Chunk::depth / 2, 1);

		if(glm::length(center) > (renderDistance + 1) * Chunk::width) {
			continue;
		}

		// Is this chunk on the screen?
		center = states.projectionMatrix.getMatrix() * center;

		float d = glm::length(center);
		center.x /= center.w;
		center.y /= center.w;

		// If it is behind the camera, don't bother drawing it
		if(center.z < -Chunk::height / 2) {
			continue;
		}

		// If it is outside the screen, don't bother drawing it
		if(fabsf(center.x) > 1 + fabsf(Chunk::height * 2 / center.w)
		|| fabsf(center.y) > 1 + fabsf(Chunk::height * 2 / center.w)) {
			continue;
		}

		// If this chunk is not initialized, skip it
		if(!it->isInitialized()) {
			// But if it is the closest to the camera, mark it for initialization
			if(d < ud) {
				ud = d;
				ux = it->x();
				uy = it->y();
				uz = it->z();
			}

			continue;
		}

		chunks.emplace_back(&*it, states.transform);
	}

	if(ud < 1000) {
		m_terrainGenerator.generate(*getChunk(ux, uy, uz));

		if(getChunk(ux, uy, uz)->getSurroundingChunk(Chunk::Left))   m_terrainGenerator.generate(*getChunk(ux, uy, uz)->getSurroundingChunk(Chunk::Left));
		if(getChunk(ux, uy, uz)->getSurroundingChunk(Chunk::Right))  m_terrainGenerator.generate(*getChunk(ux, uy, uz)->getSurroundingChunk(Chunk::Right));
		if(getChunk(ux, uy, uz)->getSurroundingChunk(Chunk::Bottom)) m_terrainGenerator.generate(*getChunk(ux, uy, uz)->getSurroundingChunk(Chunk::Bottom));
		if(getChunk(ux, uy, uz)->getSurroundingChunk(Chunk::Top))    m_terrainGenerator.generate(*getChunk(ux, uy, uz)->getSurroundingChunk(Chunk::Top));
		if(getChunk(ux, uy, uz)->getSurroundingChunk(Chunk::Front))  m_terrainGenerator.generate(*getChunk(ux, uy, uz)->getSurroundingChunk(Chunk::Front));
		if(getChunk(ux, uy, uz)->getSurroundingChunk(Chunk::Back))   m_terrainGenerator.generate(*getChunk(ux, uy, uz)->getSurroundingChunk(Chunk::Back));

		getChunk(ux, uy, uz)->setInitialized(true);
	}

	for (u8 i = 0 ; i < ChunkBuilder::layers ; ++i) {
		for (auto &it : chunks) {
			states.transform = it.second;
			it.first->drawLayer(target, states, i);
		}
	}
}

Chunk *World::getChunk(int cx, int cy, int cz) const {
	cx += m_width / 2;
	cy += m_height / 2;
	cz += m_depth / 2;

	if (cx < 0 || cx >= m_width || cy < 0 || cy >= m_height || cz < 0 || cz >= m_depth)
		return nullptr;

	return m_chunks.at(cx + cy * m_width + cz * m_width * m_height).get();
}

u16 World::getBlock(int x, int y, int z) const {
	int cx = (x + Chunk::width * (m_width / 2)) / Chunk::width;
	int cy = (y + Chunk::height * (m_height / 2)) / Chunk::height;
	int cz = (z + Chunk::depth * (m_depth / 2)) / Chunk::depth;

	if (cx < 0 || cx >= m_width || cy < 0 || cy >= m_height || cz < 0 || cz >= m_depth)
		return 0;

	Chunk *chunk = m_chunks.at(cx + cy * m_width + cz * m_width * m_height).get();
	if (chunk)
		return chunk->getBlock(x & (Chunk::width - 1), y & (Chunk::height - 1), z & (Chunk::depth - 1));
	return 0;
}

void World::setBlock(int x, int y, int z, u16 id) {
	int cx = (x + Chunk::width * (m_width / 2)) / Chunk::width;
	int cy = (y + Chunk::height * (m_height / 2)) / Chunk::height;
	int cz = (z + Chunk::depth * (m_depth / 2)) / Chunk::depth;

	if (cx < 0 || cx >= m_width || cy < 0 || cy >= m_height || cz < 0 || cz >= m_depth)
		return;

	Chunk *chunk = m_chunks.at(cx + cy * m_width + cz * m_width * m_height).get();
	if (chunk)
		chunk->setBlock(x & (Chunk::width - 1), y & (Chunk::height - 1), z & (Chunk::depth - 1), id);
}

u16 World::getData(int x, int y, int z) const {
	int cx = (x + Chunk::width * (m_width / 2)) / Chunk::width;
	int cy = (y + Chunk::height * (m_height / 2)) / Chunk::height;
	int cz = (z + Chunk::depth * (m_depth / 2)) / Chunk::depth;

	if (cx < 0 || cx >= m_width || cy < 0 || cy >= m_height || cz < 0 || cz >= m_depth)
		return 0;

	Chunk *chunk = m_chunks.at(cx + cy * m_width + cz * m_width * m_height).get();
	if (chunk)
		return chunk->getData(x & (Chunk::width - 1), y & (Chunk::height - 1), z & (Chunk::depth - 1));
	return 0;
}

void World::setData(int x, int y, int z, u16 id) {
	int cx = (x + Chunk::width * (m_width / 2)) / Chunk::width;
	int cy = (y + Chunk::height * (m_height / 2)) / Chunk::height;
	int cz = (z + Chunk::depth * (m_depth / 2)) / Chunk::depth;

	if (cx < 0 || cx >= m_width || cy < 0 || cy >= m_height || cz < 0 || cz >= m_depth)
		return;

	Chunk *chunk = m_chunks.at(cx + cy * m_width + cz * m_width * m_height).get();
	if (chunk)
		chunk->setData(x & (Chunk::width - 1), y & (Chunk::height - 1), z & (Chunk::depth - 1), id);
}

BlockData *World::getBlockData(int x, int y, int z) {
	int cx = (x + Chunk::width * (m_width / 2)) / Chunk::width;
	int cy = (y + Chunk::height * (m_height / 2)) / Chunk::height;
	int cz = (z + Chunk::depth * (m_depth / 2)) / Chunk::depth;

	if (cx < 0 || cx >= m_width || cy < 0 || cy >= m_height || cz < 0 || cz >= m_depth)
		return 0;

	Chunk *chunk = m_chunks.at(cx + cy * m_width + cz * m_width * m_height).get();
	if (chunk)
		return chunk->getBlockData(x & (Chunk::width - 1), y & (Chunk::height - 1), z & (Chunk::depth - 1));
	return 0;
}

