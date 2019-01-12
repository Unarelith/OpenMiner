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
				// FIXME
				// m_chunks.push_back(std::unique_ptr<Chunk>(new Chunk(x - m_width / 2,
				//                                                     y - m_height / 2,
				//                                                     z - m_depth / 2, m_texture)));
			}
		}
	}

	for(s32 z = -m_depth / 2 ; z < m_depth / 2 ; z++) {
		for(s32 y = -m_height / 2 ; y < m_height / 2 ; y++) {
			for(s32 x = -m_width / 2 ; x < m_width / 2 ; x++) {
				// FIXME
				// Chunk *chunk = getChunk(x, y, z);
				// if(x > -m_width / 2)      chunk->setSurroundingChunk(Chunk::Left,   getChunk(x - 1, y, z));
				// if(x <  m_width / 2 - 1)  chunk->setSurroundingChunk(Chunk::Right,  getChunk(x + 1, y, z));
				// if(y > -m_height / 2)     chunk->setSurroundingChunk(Chunk::Bottom, getChunk(x, y - 1, z));
				// if(y <  m_height / 2 - 1) chunk->setSurroundingChunk(Chunk::Top,    getChunk(x, y + 1, z));
				// if(z > -m_depth / 2)      chunk->setSurroundingChunk(Chunk::Front,  getChunk(x, y, z - 1));
				// if(z <  m_depth / 2 - 1)  chunk->setSurroundingChunk(Chunk::Back,   getChunk(x, y, z + 1));
			}
		}
	}
}

void World::update() {
	// FIXME
	// for (auto &it : m_chunks) {
	// 	if (isReloadRequested)
	// 		it->setChanged(true);
    //
	// 	it->update(player, *this);
	// }

	isReloadRequested = false;
}

void World::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	if (!target.getView()) {
		DEBUG("ERROR: Trying to draw world without a camera");
		return;
	}

	float ud = 1000.0;
	int ux = 0;
	int uy = 0;
	int uz = 0;

	gk::Shader::bind(states.shader);
	states.shader->setUniform("u_renderDistance", renderDistance * CHUNK_WIDTH);
	gk::Shader::bind(nullptr);

	std::vector<std::pair<Chunk*, gk::Transform>> chunks;
	for(auto &it : m_chunks) {
		// FIXME
		// states.transform = glm::translate(glm::mat4(1.0f),
		//                                   glm::vec3(it->x() * CHUNK_WIDTH,
		//                                             it->y() * CHUNK_HEIGHT,
		//                                             it->z() * CHUNK_DEPTH));

		// Is the chunk close enough?
		glm::vec4 center = target.getView()->getViewTransform().getMatrix()
		                 * states.transform.getMatrix()
		                 * glm::vec4(CHUNK_WIDTH / 2, CHUNK_HEIGHT / 2, CHUNK_DEPTH / 2, 1);

		if(glm::length(center) > (renderDistance + 1) * CHUNK_WIDTH) {
			continue;
		}

		// Is this chunk on the screen?
		center = target.getView()->getTransform().getMatrix() * center;

		float d = glm::length(center);
		center.x /= center.w;
		center.y /= center.w;

		// If it is behind the camera, don't bother drawing it
		if(center.z < -CHUNK_HEIGHT / 2) {
			continue;
		}

		// If it is outside the screen, don't bother drawing it
		if(fabsf(center.x) > 1 + fabsf(CHUNK_HEIGHT * 2 / center.w)
		|| fabsf(center.y) > 1 + fabsf(CHUNK_HEIGHT * 2 / center.w)) {
			continue;
		}

		// If this chunk is not initialized, skip it
		if(!it->isInitialized()) {
			// But if it is the closest to the camera, mark it for initialization
			if(d < ud) {
				ud = d;
				// FIXME
				// ux = it->x();
				// uy = it->y();
				// uz = it->z();
			}

			continue;
		}

		chunks.emplace_back(&*it, states.transform);
	}

	if(ud < 1000) {
		// FIXME
		// m_terrainGenerator.generate(*getChunk(ux, uy, uz));

		// FIXME
		// if(getChunk(ux, uy, uz)->getSurroundingChunk(Chunk::Left))   m_terrainGenerator.generate(*getChunk(ux, uy, uz)->getSurroundingChunk(Chunk::Left));
		// if(getChunk(ux, uy, uz)->getSurroundingChunk(Chunk::Right))  m_terrainGenerator.generate(*getChunk(ux, uy, uz)->getSurroundingChunk(Chunk::Right));
		// if(getChunk(ux, uy, uz)->getSurroundingChunk(Chunk::Bottom)) m_terrainGenerator.generate(*getChunk(ux, uy, uz)->getSurroundingChunk(Chunk::Bottom));
		// if(getChunk(ux, uy, uz)->getSurroundingChunk(Chunk::Top))    m_terrainGenerator.generate(*getChunk(ux, uy, uz)->getSurroundingChunk(Chunk::Top));
		// if(getChunk(ux, uy, uz)->getSurroundingChunk(Chunk::Front))  m_terrainGenerator.generate(*getChunk(ux, uy, uz)->getSurroundingChunk(Chunk::Front));
		// if(getChunk(ux, uy, uz)->getSurroundingChunk(Chunk::Back))   m_terrainGenerator.generate(*getChunk(ux, uy, uz)->getSurroundingChunk(Chunk::Back));

		getChunk(ux, uy, uz)->setInitialized(true);
	}

	// FIXME
	// for (u8 i = 0 ; i < ChunkBuilder::layers ; ++i) {
	// 	for (auto &it : chunks) {
	// 		states.transform = it.second;
	// 		it.first->drawLayer(target, states, i);
	// 	}
	// }
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
	int cx = (x + CHUNK_WIDTH * (m_width / 2)) / CHUNK_WIDTH;
	int cy = (y + CHUNK_HEIGHT * (m_height / 2)) / CHUNK_HEIGHT;
	int cz = (z + CHUNK_DEPTH * (m_depth / 2)) / CHUNK_DEPTH;

	if (cx < 0 || cx >= m_width || cy < 0 || cy >= m_height || cz < 0 || cz >= m_depth)
		return 0;

	// FIXME
	// Chunk *chunk = m_chunks.at(cx + cy * m_width + cz * m_width * m_height).get();
	// if (chunk)
	// 	return chunk->getBlock(x & (CHUNK_WIDTH - 1), y & (CHUNK_HEIGHT - 1), z & (CHUNK_DEPTH - 1));
	return 0;
}

void World::setBlock(int x, int y, int z, u16 id) {
	int cx = (x + CHUNK_WIDTH * (m_width / 2)) / CHUNK_WIDTH;
	int cy = (y + CHUNK_HEIGHT * (m_height / 2)) / CHUNK_HEIGHT;
	int cz = (z + CHUNK_DEPTH * (m_depth / 2)) / CHUNK_DEPTH;

	if (cx < 0 || cx >= m_width || cy < 0 || cy >= m_height || cz < 0 || cz >= m_depth)
		return;

	// FIXME
	// Chunk *chunk = m_chunks.at(cx + cy * m_width + cz * m_width * m_height).get();
	// if (chunk)
	// 	chunk->setBlock(x & (CHUNK_WIDTH - 1), y & (CHUNK_HEIGHT - 1), z & (CHUNK_DEPTH - 1), id);
}

u16 World::getData(int x, int y, int z) const {
	int cx = (x + CHUNK_WIDTH * (m_width / 2)) / CHUNK_WIDTH;
	int cy = (y + CHUNK_HEIGHT * (m_height / 2)) / CHUNK_HEIGHT;
	int cz = (z + CHUNK_DEPTH * (m_depth / 2)) / CHUNK_DEPTH;

	if (cx < 0 || cx >= m_width || cy < 0 || cy >= m_height || cz < 0 || cz >= m_depth)
		return 0;

	// FIXME
	// Chunk *chunk = m_chunks.at(cx + cy * m_width + cz * m_width * m_height).get();
	// if (chunk)
	// 	return chunk->getData(x & (CHUNK_WIDTH - 1), y & (CHUNK_HEIGHT - 1), z & (CHUNK_DEPTH - 1));
	return 0;
}

void World::setData(int x, int y, int z, u16 id) {
	int cx = (x + CHUNK_WIDTH * (m_width / 2)) / CHUNK_WIDTH;
	int cy = (y + CHUNK_HEIGHT * (m_height / 2)) / CHUNK_HEIGHT;
	int cz = (z + CHUNK_DEPTH * (m_depth / 2)) / CHUNK_DEPTH;

	if (cx < 0 || cx >= m_width || cy < 0 || cy >= m_height || cz < 0 || cz >= m_depth)
		return;

	// FIXME
	// Chunk *chunk = m_chunks.at(cx + cy * m_width + cz * m_width * m_height).get();
	// if (chunk)
	// 	chunk->setData(x & (CHUNK_WIDTH - 1), y & (CHUNK_HEIGHT - 1), z & (CHUNK_DEPTH - 1), id);
}

BlockData *World::getBlockData(int x, int y, int z) {
	int cx = (x + CHUNK_WIDTH * (m_width / 2)) / CHUNK_WIDTH;
	int cy = (y + CHUNK_HEIGHT * (m_height / 2)) / CHUNK_HEIGHT;
	int cz = (z + CHUNK_DEPTH * (m_depth / 2)) / CHUNK_DEPTH;

	if (cx < 0 || cx >= m_width || cy < 0 || cy >= m_height || cz < 0 || cz >= m_depth)
		return 0;

	// FIXME
	// Chunk *chunk = m_chunks.at(cx + cy * m_width + cz * m_width * m_height).get();
	// if (chunk)
	// 	return chunk->getBlockData(x & (CHUNK_WIDTH - 1), y & (CHUNK_HEIGHT - 1), z & (CHUNK_DEPTH - 1));
	return 0;
}

