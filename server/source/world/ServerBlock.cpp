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
#include "ServerPlayer.hpp"
#include "Server.hpp"
#include "World.hpp"

void ServerBlock::onTick(const glm::ivec3 &pos, std::unordered_map<u16, ServerPlayer> &players, Chunk &chunk, World &world, Server &server) const {
	try {
		if (m_onTick && m_onTickEnabled) {
			m_onTick(pos, players, chunk, world);

			// FIXME: Send this every 0.5 second instead of once per tick
			// u16 id = world.getBlock(pos.x, pos.y, pos.z);
			// u16 data = world.getData(pos.x, pos.y, pos.z);
			BlockData *blockData = world.getBlockData(pos.x, pos.y, pos.z);
			if (blockData) {
				// sf::Packet packet;
				// packet << Network::Command::BlockUpdate << s32(pos.x) << s32(pos.y) << s32(pos.z)
				// 	<< (id | (data << 16));
				// server.sendToAllClients(packet);

				sf::Packet packet1;
				packet1 << Network::Command::BlockDataUpdate << s32(pos.x) << s32(pos.y) << s32(pos.z);
				packet1 << blockData->data << blockData->useAltTiles;
				server.sendToAllClients(packet1);

				if (blockData->inventory.hasChanged()) {
					sf::Packet packet2;
					packet2 << Network::Command::BlockInvUpdate;
					packet2 << s32(pos.x) << s32(pos.y) << s32(pos.z);
					packet2 << blockData->inventory;
					server.sendToAllClients(packet2);
					blockData->inventory.setChanged(false);
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

