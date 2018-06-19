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

#include "Camera.hpp"
#include "Config.hpp"
#include "World.hpp"

World::World() {
	m_texture.load("textures/blocks.png");

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
				if(x > -m_width / 2)      getChunk(x, y, z)->setLeft(getChunk(x - 1, y, z));
				if(x <  m_width / 2 - 1)  getChunk(x, y, z)->setRight(getChunk(x + 1, y, z));
				if(y > -m_height / 2)     getChunk(x, y, z)->setBelow(getChunk(x, y - 1, z));
				if(y <  m_height / 2 - 1) getChunk(x, y, z)->setAbove(getChunk(x, y + 1, z));
				if(z > -m_depth / 2)      getChunk(x, y, z)->setFront(getChunk(x, y, z - 1));
				if(z <  m_depth / 2 - 1)  getChunk(x, y, z)->setBack(getChunk(x, y, z + 1));
			}
		}
	}
}

void World::updateChunks() {
	// FIXME
	for (auto &it : m_chunks) {
		it->update();
	}
}

void World::draw(RenderTarget &target, RenderStates states) const {
	float ud = 1000.0;
	int ux = 0;
	int uy = 0;
	int uz = 0;

	Shader::bind(states.shader);
	states.shader->setUniform("u_renderDistance", renderDistance * Chunk::width);
	Shader::bind(nullptr);

	for(auto &it : m_chunks) {
		glm::mat4 modelMatrix{glm::translate(glm::mat4(1.0f),
		                                     glm::vec3(it->x() * Chunk::width,
		                                               it->y() * Chunk::height,
		                                               it->z() * Chunk::depth))};

		states.modelMatrix = &modelMatrix;

		// Is the chunk close enough?
		glm::vec4 center = *states.viewMatrix * *states.modelMatrix * glm::vec4(Chunk::width  / 2,
		                                                                        Chunk::height / 2,
		                                                                        Chunk::depth  / 2, 1);

		if(glm::length(center) > (renderDistance + 1) * Chunk::width) {
			continue;
		}

		// Is this chunk on the screen?
		center = *states.projectionMatrix * center;

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

		Shader::bind(states.shader);
		// states.shader->setUniform("u_modelMatrix", modelMatrix);
		target.draw(*it, states);
		Shader::bind(nullptr);
	}

	if(ud < 1000) {
		m_terrainGenerator.generate(*getChunk(ux, uy, uz));

		if(getChunk(ux, uy, uz)->left())  m_terrainGenerator.generate(*getChunk(ux, uy, uz)->left());
		if(getChunk(ux, uy, uz)->right()) m_terrainGenerator.generate(*getChunk(ux, uy, uz)->right());
		if(getChunk(ux, uy, uz)->below()) m_terrainGenerator.generate(*getChunk(ux, uy, uz)->below());
		if(getChunk(ux, uy, uz)->above()) m_terrainGenerator.generate(*getChunk(ux, uy, uz)->above());
		if(getChunk(ux, uy, uz)->front()) m_terrainGenerator.generate(*getChunk(ux, uy, uz)->front());
		if(getChunk(ux, uy, uz)->back())  m_terrainGenerator.generate(*getChunk(ux, uy, uz)->back());

		getChunk(ux, uy, uz)->setInitialized(true);
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

Block *World::getBlock(int x, int y, int z) const {
	s32 cx = (x + Chunk::width * (m_width / 2)) / Chunk::width;
	s32 cy = (y + Chunk::height * (m_height / 2)) / Chunk::height;
	s32 cz = (z + Chunk::depth * (m_depth / 2)) / Chunk::depth;

	if (cx < 0 || cx >= m_width || cy < 0 || cy >= m_height || cz < 0 || cz >= m_depth)
		return nullptr;

	Chunk *chunk = m_chunks.at(cx + cy * m_width + cz * m_width * m_height).get();
	if (chunk)
		return chunk->getBlock(x & (Chunk::width - 1), y & (Chunk::height - 1), z & (Chunk::depth - 1));
	return nullptr;
}

void World::setBlock(int x, int y, int z, u32 id) {
	s32 cx = (x + Chunk::width * (m_width / 2)) / Chunk::width;
	s32 cy = (y + Chunk::height * (m_height / 2)) / Chunk::height;
	s32 cz = (z + Chunk::depth * (m_depth / 2)) / Chunk::depth;

	if (cx < 0 || cx >= m_width || cy < 0 || cy >= m_height || cz < 0 || cz >= m_depth)
		return;

	Chunk *chunk = m_chunks.at(cx + cy * m_width + cz * m_width * m_height).get();
	if (chunk)
		chunk->setBlock(x & (Chunk::width - 1), y & (Chunk::height - 1), z & (Chunk::depth - 1), id);
}

