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
#include <glm/gtc/matrix_transform.hpp>

#include <gk/gl/Shader.hpp>
#include <gk/resource/ResourceHandler.hpp>

#include "ClientCommandHandler.hpp"
#include "ClientWorld.hpp"
#include "World.hpp"

ClientWorld::ClientWorld() : m_texture(gk::ResourceHandler::getInstance().get<gk::Texture>("texture-blocks")) {
}

void ClientWorld::update() {
	for (auto &it : m_chunks) {
		if (World::isReloadRequested)
			it.second->setChanged(true);

		it.second->update();
	}

	World::isReloadRequested = false;
}

void ClientWorld::receiveChunkData(sf::Packet &packet) {
	s32 cx, cy, cz;
	packet >> cx >> cy >> cz;

	Chunk *chunk = getChunk(cx, cy, cz);
	if (!chunk) {
		auto it = m_chunks.emplace(gk::Vector3i{cx, cy, cz}, new ClientChunk(cx, cy, cz, m_texture));
		chunk = it.first->second.get();
	}

	for (u16 z = 0 ; z < CHUNK_DEPTH ; ++z) {
		for (u16 y = 0 ; y < CHUNK_HEIGHT ; ++y) {
			for (u16 x = 0 ; x < CHUNK_WIDTH ; ++x) {
				u16 block;
				u8 light;

				packet >> block >> light;

				chunk->setBlockRaw(x, y, z, block & 0xffff);
				// chunk->setData(x, y, z, block >> 16);
				chunk->lightmap().setLightData(x, y, z, light);
			}
		}
	}

	chunk->setInitialized(true);

	// if(chunk->getSurroundingChunk(Chunk::Left))   chunk->getSurroundingChunk(Chunk::Left)->setChanged(true);
	// if(chunk->getSurroundingChunk(Chunk::Right))  chunk->getSurroundingChunk(Chunk::Right)->setChanged(true);
	// if(chunk->getSurroundingChunk(Chunk::Bottom)) chunk->getSurroundingChunk(Chunk::Bottom)->setChanged(true);
	// if(chunk->getSurroundingChunk(Chunk::Top))    chunk->getSurroundingChunk(Chunk::Top)->setChanged(true);
	// if(chunk->getSurroundingChunk(Chunk::Front))  chunk->getSurroundingChunk(Chunk::Front)->setChanged(true);
	// if(chunk->getSurroundingChunk(Chunk::Back))   chunk->getSurroundingChunk(Chunk::Back)->setChanged(true);

	// std::cout << "Chunk at (" << cx << ", " << cy << ", " << cz << ") received" << std::endl;
}

Chunk *ClientWorld::getChunk(int cx, int cy, int cz) const {
	auto it = m_chunks.find({cx, cy, cz});
	if (it == m_chunks.end())
		return nullptr;

	return it->second.get();
}

void ClientWorld::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	if (!target.getView()) {
		DEBUG("ERROR: Trying to draw world without a camera");
		return;
	}

	gk::Shader::bind(states.shader);
	states.shader->setUniform("u_renderDistance", Config::renderDistance * CHUNK_WIDTH);
	gk::Shader::bind(nullptr);

	std::vector<std::pair<ClientChunk*, gk::Transform>> chunks;
	for(auto &it : m_chunks) {
		states.transform = glm::translate(glm::mat4(1.0f),
		                                  glm::vec3(it.second->x() * CHUNK_WIDTH,
		                                            it.second->y() * CHUNK_HEIGHT,
		                                            it.second->z() * CHUNK_DEPTH));

		// Is the chunk close enough?
		glm::vec4 center = target.getView()->getViewTransform().getMatrix()
		                 * states.transform.getMatrix()
		                 * glm::vec4(CHUNK_WIDTH / 2, CHUNK_HEIGHT / 2, CHUNK_DEPTH / 2, 1);

		// Nope, too far, don't render it
		if(glm::length(center) > (Config::renderDistance + 1) * CHUNK_WIDTH) {
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
		if(!it.second->isInitialized()) {
			continue;
		}

		chunks.emplace_back(it.second.get(), states.transform);
	}

	for (u8 i = 0 ; i < ChunkBuilder::layers ; ++i) {
		for (auto &it : chunks) {
			states.transform = it.second;
			it.first->drawLayer(target, states, i);
		}
	}
}

