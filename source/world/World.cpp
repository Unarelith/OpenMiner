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

// Block *World::selectedBlock = nullptr;
// Chunk *World::selectedChunk = nullptr;

World::World() {
	//m_texture.load("textures/cobblestone.bmp");
	m_texture.load("textures/blocks.png");

	for(s32 z = -m_depth / 2 ; z < m_depth / 2 ; z++) {
		for(s32 x = -m_width / 2 ; x < m_width / 2 ; x++) {
			m_chunks.push_back(std::unique_ptr<Chunk>(new Chunk(x, 0, z, m_texture)));
		}
	}

	for(s32 z = -m_depth / 2 ; z < m_depth / 2 ; z++) {
		for(s32 x = -m_width / 2 ; x < m_width / 2 ; x++) {
			if(x > -m_width / 2)     getChunk(x, z)->setLeft(getChunk(x - 1, z));
			if(x <  m_width / 2 - 1) getChunk(x, z)->setRight(getChunk(x + 1, z));
			if(z > -m_depth / 2)     getChunk(x, z)->setFront(getChunk(x, z - 1));
			if(z <  m_depth / 2 - 1) getChunk(x, z)->setBack(getChunk(x, z + 1));
		}
	}
}

void World::draw(Shader &shader, const glm::mat4 &projectionMatrix, const glm::mat4 &viewMatrix) {
	float ud = 1000.0;
	int ux = 0;
	// int uy = 0;
	int uz = 0;

	shader.setUniform("u_renderDistance", renderDistance * Chunk::width);

	for(auto &it : m_chunks) {
		glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f),
		                                       glm::vec3(it->x() * Chunk::width,
		                                                 it->y() * Chunk::height,
		                                                 it->z() * Chunk::depth));

		// Is the chunk close enough?
		glm::vec4 center = viewMatrix * modelMatrix * glm::vec4(Chunk::width  / 2,
		                                                        Chunk::height / 2,
		                                                        Chunk::depth  / 2, 1);

		if(glm::length(center) > (renderDistance + 1) * Chunk::width) {
			continue;
		}

		// Is this chunk on the screen?
		center = projectionMatrix * center;

		float d = glm::length(center);
		center.x /= center.w;
		center.y /= center.w;

		// If it is behind the camera, don't bother drawing it
		if(center.z < -Chunk::height / 2) {
			continue;
		}

		// It it is outside the screen, don't bother drawing it
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
				// ux = it->y();
				uz = it->z();
			}

			continue;
		}

		shader.setUniform("u_modelMatrix", modelMatrix);

		it->draw(shader);
	}

	if(ud < 1000) {
		m_terrainGenerator.generate(*getChunk(ux, uz));

		if(getChunk(ux, uz)->left())  m_terrainGenerator.generate(*getChunk(ux, uz)->left());
		if(getChunk(ux, uz)->right()) m_terrainGenerator.generate(*getChunk(ux, uz)->right());
		if(getChunk(ux, uz)->front()) m_terrainGenerator.generate(*getChunk(ux, uz)->front());
		if(getChunk(ux, uz)->back())  m_terrainGenerator.generate(*getChunk(ux, uz)->back());

		getChunk(ux, uz)->setInitialized(true);
	}
}

Chunk *World::getChunk(int cx, int cz) {
	cx += m_width / 2;
	cz += m_depth / 2;

	if (cx < 0 || cx >= m_width /*|| cy < 0 || cy > m_height */ || cz < 0 || cz >= m_depth)
		return nullptr;

	return m_chunks.at(cx + cz * m_width).get();
}

Block *World::getBlock(int x, int y, int z) {
	s32 cx = (x + Chunk::width * (m_width / 2)) / Chunk::width;
	s32 cy = (y + Chunk::height * (m_height / 2)) / Chunk::height;
	s32 cz = (z + Chunk::depth * (m_depth / 2)) / Chunk::depth;

	if (cx < 0 || cx >= m_width || cy < 0 || cy > m_height || cz < 0 || cz >= m_depth)
		return nullptr;

	Chunk *chunk = m_chunks.at(cx + cz * m_width).get();
	if (chunk)
		return chunk->getBlock(x & (Chunk::width - 1), y & (Chunk::height - 1), z & (Chunk::depth - 1));
	return nullptr;
}

void World::setBlock(int x, int y, int z, u32 id) {
	s32 cx = (x + Chunk::width * (m_width / 2)) / Chunk::width;
	s32 cy = (y + Chunk::height * (m_height / 2)) / Chunk::height;
	s32 cz = (z + Chunk::depth * (m_depth / 2)) / Chunk::depth;

	if (cx < 0 || cx >= m_width || cy < 0 || cy > m_height || cz < 0 || cz >= m_depth)
		return;

	Chunk *chunk = m_chunks.at(cx + cz * m_width).get();
	if (chunk)
		chunk->setBlock(x & (Chunk::width - 1), y & (Chunk::height - 1), z & (Chunk::depth - 1), id);
}

