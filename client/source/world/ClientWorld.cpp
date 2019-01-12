/*
 * =====================================================================================
 *
 *       Filename:  ClientWorld.cpp
 *
 *    Description:
 *
 *        Created:  12/01/2019 14:01:17
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include <gk/gl/Shader.hpp>
#include <gk/resource/ResourceHandler.hpp>

#include "Config.hpp"
#include "ClientWorld.hpp"
#include "World.hpp"

ClientWorld::ClientWorld() : m_texture(gk::ResourceHandler::getInstance().get<gk::Texture>("texture-blocks")) {
	for(s32 z = 0 ; z < m_depth ; z++) {
		for(s32 y = 0 ; y < m_height ; y++) {
			for(s32 x = 0 ; x < m_width ; x++) {
				m_chunks.emplace_back(new ClientChunk(x - m_width / 2,
				                                      y - m_height / 2,
				                                      z - m_depth / 2, m_texture));
			}
		}
	}

	for(s32 z = -m_depth / 2 ; z < m_depth / 2 ; z++) {
		for(s32 y = -m_height / 2 ; y < m_height / 2 ; y++) {
			for(s32 x = -m_width / 2 ; x < m_width / 2 ; x++) {
				ClientChunk *chunk = getChunk(x, y, z);
				if(x > -m_width / 2)      chunk->setSurroundingChunk(ClientChunk::Left,   getChunk(x - 1, y, z));
				if(x <  m_width / 2 - 1)  chunk->setSurroundingChunk(ClientChunk::Right,  getChunk(x + 1, y, z));
				if(y > -m_height / 2)     chunk->setSurroundingChunk(ClientChunk::Bottom, getChunk(x, y - 1, z));
				if(y <  m_height / 2 - 1) chunk->setSurroundingChunk(ClientChunk::Top,    getChunk(x, y + 1, z));
				if(z > -m_depth / 2)      chunk->setSurroundingChunk(ClientChunk::Front,  getChunk(x, y, z - 1));
				if(z <  m_depth / 2 - 1)  chunk->setSurroundingChunk(ClientChunk::Back,   getChunk(x, y, z + 1));
			}
		}
	}
}

void ClientWorld::update() {
	for (auto &it : m_chunks)
		it->update();
}

void ClientWorld::receiveChunkData(sf::Packet &packet) {
	s32 cx, cy, cz;
	packet >> cx >> cy >> cz;

	ClientChunk *chunk = getChunk(cx, cy, cz);
	if (chunk) {
		for (u16 z = 0 ; z < CHUNK_DEPTH ; ++z) {
			for (u16 y = 0 ; y < CHUNK_HEIGHT ; ++y) {
				for (u16 x = 0 ; x < CHUNK_WIDTH ; ++x) {
					u16 block;
					packet >> block;

					chunk->setBlock(x, y, z, block);
					chunk->lightmap().addSunlight(x, y, z, 15);
				}
			}
		}

		chunk->setInitialized(true);

		// std::cout << "Chunk at (" << cx << ", " << cy << ", " << cz << ") received" << std::endl;
	}
}

ClientChunk *ClientWorld::getChunk(int cx, int cy, int cz) const {
	cx += m_width / 2;
	cy += m_height / 2;
	cz += m_depth / 2;

	if (cx < 0 || cx >= m_width || cy < 0 || cy >= m_height || cz < 0 || cz >= m_depth)
		return nullptr;

	return m_chunks.at(cx + cy * m_width + cz * m_width * m_height).get();
}

void ClientWorld::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	if (!target.getView()) {
		DEBUG("ERROR: Trying to draw world without a camera");
		return;
	}

	gk::Shader::bind(states.shader);
	states.shader->setUniform("u_renderDistance", World::renderDistance * CHUNK_WIDTH);
	gk::Shader::bind(nullptr);

	// float ud = 1000.0;
	// int ux = 0;
	// int uy = 0;
	// int uz = 0;

	std::vector<std::pair<ClientChunk*, gk::Transform>> chunks;
	for(auto &it : m_chunks) {
		states.transform = glm::translate(glm::mat4(1.0f),
		                                  glm::vec3(it->x() * CHUNK_WIDTH,
		                                            it->y() * CHUNK_HEIGHT,
		                                            it->z() * CHUNK_DEPTH));

		// Is the chunk close enough?
		glm::vec4 center = target.getView()->getViewTransform().getMatrix()
		                 * states.transform.getMatrix()
		                 * glm::vec4(CHUNK_WIDTH / 2, CHUNK_HEIGHT / 2, CHUNK_DEPTH / 2, 1);

		if(glm::length(center) > (World::renderDistance + 1) * CHUNK_WIDTH) {
			continue;
		}

		// Is this chunk on the screen?
		center = target.getView()->getTransform().getMatrix() * center;

		// float d = glm::length(center);
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
			// FIXME
			// if(d < ud) {
			// 	ud = d;
			// 	ux = it->x();
			// 	uy = it->y();
			// 	uz = it->z();
			// }

			continue;
		}

		chunks.emplace_back(&*it, states.transform);
	}

	// FIXME
	// if(ud < 1000) {
	// 	m_terrainGenerator.generate(*getChunk(ux, uy, uz));
    //
	// 	if(getChunk(ux, uy, uz)->getSurroundingChunk(Chunk::Left))   m_terrainGenerator.generate(*getChunk(ux, uy, uz)->getSurroundingChunk(Chunk::Left));
	// 	if(getChunk(ux, uy, uz)->getSurroundingChunk(Chunk::Right))  m_terrainGenerator.generate(*getChunk(ux, uy, uz)->getSurroundingChunk(Chunk::Right));
	// 	if(getChunk(ux, uy, uz)->getSurroundingChunk(Chunk::Bottom)) m_terrainGenerator.generate(*getChunk(ux, uy, uz)->getSurroundingChunk(Chunk::Bottom));
	// 	if(getChunk(ux, uy, uz)->getSurroundingChunk(Chunk::Top))    m_terrainGenerator.generate(*getChunk(ux, uy, uz)->getSurroundingChunk(Chunk::Top));
	// 	if(getChunk(ux, uy, uz)->getSurroundingChunk(Chunk::Front))  m_terrainGenerator.generate(*getChunk(ux, uy, uz)->getSurroundingChunk(Chunk::Front));
	// 	if(getChunk(ux, uy, uz)->getSurroundingChunk(Chunk::Back))   m_terrainGenerator.generate(*getChunk(ux, uy, uz)->getSurroundingChunk(Chunk::Back));
    //
	// 	getChunk(ux, uy, uz)->setInitialized(true);
	// }

	for (u8 i = 0 ; i < ChunkBuilder::layers ; ++i) {
		for (auto &it : chunks) {
			states.transform = it.second;
			it.first->drawLayer(target, states, i);
		}
	}
}

