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
#include "Player.hpp"
#include "World.hpp"

ClientWorld::ClientWorld() : m_chunk(0, 0, 2, gk::ResourceHandler::getInstance().get<gk::Texture>("texture-blocks")) {
}

void ClientWorld::update(Player &player) {
	m_chunk.update();
}

void ClientWorld::receiveChunkData(sf::Packet &packet) {
	for (u16 z = 0 ; z < CHUNK_DEPTH ; ++z) {
		for (u16 y = 0 ; y < CHUNK_HEIGHT ; ++y) {
			for (u16 x = 0 ; x < CHUNK_WIDTH ; ++x) {
				u16 block;
				packet >> block;
				m_chunk.setBlock(x, y, z, block);
				m_chunk.lightmap().addSunlight(x, y, z, 15);
			}
		}
	}
}

void ClientWorld::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	gk::Shader::bind(states.shader);
	states.shader->setUniform("u_renderDistance", World::renderDistance * CHUNK_WIDTH);
	gk::Shader::bind(nullptr);

	target.draw(m_chunk, states);
}

