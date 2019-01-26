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
#include "Server.hpp"
#include "World.hpp"

void ServerBlock::onTick(const glm::ivec3 &pos, Player &player, Chunk &chunk, World &world, Server &server) const {
	try {
		if (m_onTick && m_onTickEnabled) {
			m_onTick(pos, player, chunk, world);

			// FIXME: Send this every 0.5 second instead of once per tick
			BlockData *data = world.getBlockData(pos.x, pos.y, pos.z);
			if (data) {
				sf::Packet packet;
				packet << Network::Command::BlockDataUpdate << s32(pos.x) << s32(pos.y) << s32(pos.z);
				packet << data->data;
				server.sendToAllClients(packet);

				if (data->inventory.hasChanged()) {
					sf::Packet packet2;
					packet2 << Network::Command::BlockInvUpdate;
					packet2 << s32(pos.x) << s32(pos.y) << s32(pos.z);
					packet2 << data->inventory;
					server.sendToAllClients(packet2);
					data->inventory.setChanged(false);
					DEBUG("Sending inventory update", pos.x, pos.y, pos.z);
				}
			}
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

