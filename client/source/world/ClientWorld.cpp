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

#include "Client.hpp"
#include "ClientWorld.hpp"
#include "World.hpp"

ClientWorld::ClientWorld(Client &client)
	: m_texture(gk::ResourceHandler::getInstance().get<gk::Texture>("texture-blocks")),
	  m_client(client)
{
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
	for (auto &it : m_chunks) {
		if (World::isReloadRequested)
			it->setChanged(true);

		it->update();
	}

	World::isReloadRequested = false;
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
					u8 light;

					packet >> block >> light;

					chunk->setBlockRaw(x, y, z, block & 0xffff);
					// chunk->setData(x, y, z, block >> 16);
					chunk->lightmap().setLightData(x, y, z, light);
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

u16 ClientWorld::getBlock(int x, int y, int z) const {
	int cx = (x + CHUNK_WIDTH * (m_width / 2)) / CHUNK_WIDTH;
	int cy = (y + CHUNK_HEIGHT * (m_height / 2)) / CHUNK_HEIGHT;
	int cz = (z + CHUNK_DEPTH * (m_depth / 2)) / CHUNK_DEPTH;

	if (cx < 0 || cx >= m_width || cy < 0 || cy >= m_height || cz < 0 || cz >= m_depth)
		return 0;

	ClientChunk *chunk = m_chunks.at(cx + cy * m_width + cz * m_width * m_height).get();
	if (chunk)
		return chunk->getBlock(x & (CHUNK_WIDTH - 1), y & (CHUNK_HEIGHT - 1), z & (CHUNK_DEPTH - 1));
	return 0;
}

void ClientWorld::setBlock(int x, int y, int z, u16 id) {
	int cx = (x + CHUNK_WIDTH * (m_width / 2)) / CHUNK_WIDTH;
	int cy = (y + CHUNK_HEIGHT * (m_height / 2)) / CHUNK_HEIGHT;
	int cz = (z + CHUNK_DEPTH * (m_depth / 2)) / CHUNK_DEPTH;

	if (cx < 0 || cx >= m_width || cy < 0 || cy >= m_height || cz < 0 || cz >= m_depth)
		return;

	ClientChunk *chunk = m_chunks.at(cx + cy * m_width + cz * m_width * m_height).get();
	if (chunk)
		chunk->setBlock(x & (CHUNK_WIDTH - 1), y & (CHUNK_HEIGHT - 1), z & (CHUNK_DEPTH - 1), id);
}

void ClientWorld::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	if (!target.getView()) {
		DEBUG("ERROR: Trying to draw world without a camera");
		return;
	}

	gk::Shader::bind(states.shader);
	states.shader->setUniform("u_renderDistance", Config::renderDistance * CHUNK_WIDTH);
	gk::Shader::bind(nullptr);

	float ud = 1000.0;
	s32 ux = 0;
	s32 uy = 0;
	s32 uz = 0;

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

		if(glm::length(center) > (Config::renderDistance + 1) * CHUNK_WIDTH) {
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
				ux = it->x();
				uy = it->y();
				uz = it->z();
			}

			continue;
		}

		chunks.emplace_back(&*it, states.transform);
	}

	if(ud < 1000) {
		sf::Packet packet;
		packet << Network::Command::ChunkRequest;
		packet << ux << uy << uz;
		m_client.send(packet);
	}

	for (u8 i = 0 ; i < ChunkBuilder::layers ; ++i) {
		for (auto &it : chunks) {
			states.transform = it.second;
			it.first->drawLayer(target, states, i);
		}
	}
}

