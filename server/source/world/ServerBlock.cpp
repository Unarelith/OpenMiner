/*
 * =====================================================================================
 *
 *       Filename:  ServerBlock.cpp
 *
 *    Description:
 *
 *        Created:  25/01/2019 00:20:59
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "Chunk.hpp"
#include "Player.hpp"
#include "ServerBlock.hpp"
#include "ServerInfo.hpp"
#include "World.hpp"

void ServerBlock::onTick(const glm::ivec3 &pos, Player &player, Chunk &chunk, World &world) const {
	try {
		if (m_onTick && m_onTickEnabled) {
			m_onTick(pos, player, chunk, world);
		}
	}
	catch (const sol::error &e) {
		m_onTickEnabled = false;
		std::cerr << e.what() << std::endl;
		std::cerr << "Block stopped ticking at (" << pos.x << ", " << pos.y << ", " << pos.z << ")" << std::endl;
	}
}

bool ServerBlock::onBlockActivated(const glm::ivec3 &pos, Player &player, World &world, Client &client) const {
	try {
		if (m_onBlockActivated) {
			m_onBlockActivated(pos, player, world, client);
			return true;
		}
	}
	catch (const sol::error &e) {
		std::cerr << e.what() << std::endl;
	}

	return false;
}

