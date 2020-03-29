/*
 * =====================================================================================
 *
 *  OpenMiner
 *
 *  Copyright (C) 2018-2020 Unarelith, Quentin Bazin <openminer@unarelith.net>
 *  Copyright (C) 2019-2020 the OpenMiner contributors (see CONTRIBUTORS.md)
 *
 *  This file is part of OpenMiner.
 *
 *  OpenMiner is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  OpenMiner is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with OpenMiner; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *
 * =====================================================================================
 */
#include "Chunk.hpp"
#include "Player.hpp"
#include "ServerBlock.hpp"
#include "ServerCommandHandler.hpp"
#include "ServerPlayer.hpp"
#include "World.hpp"

void ServerBlock::onTick(const glm::ivec3 &pos, Chunk &chunk, World &world, ServerCommandHandler &server) const {
	try {
		if (m_onTick && m_onTickEnabled) {
			m_onTick(pos, chunk, world);

			BlockData *blockData = world.getBlockData(pos.x, pos.y, pos.z);
			if (blockData) {
				// FIXME: Check if data changed before sending
				server.sendBlockDataUpdate(pos.x, pos.y, pos.z, blockData);

				if (blockData->inventory.hasChanged()) {
					server.sendBlockInvUpdate(pos.x, pos.y, pos.z, blockData->inventory);
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

bool ServerBlock::onBlockActivated(const glm::ivec3 &pos, Player &player, World &world, ClientInfo &client, ServerCommandHandler &server, u16 screenWidth, u16 screenHeight, u8 guiScale) const {
	try {
		if (m_onBlockActivated) {
			m_onBlockActivated(pos, player, world, client, server, screenWidth, screenHeight, guiScale);
			return true;
		}
	}
	catch (const sol::error &e) {
		std::cerr << e.what() << std::endl;
	}

	return false;
}

void ServerBlock::onBlockPlaced(const glm::ivec3 &pos, World &world) const {
	try {
		if (m_onBlockPlaced) {
			m_onBlockPlaced(pos, world);
		}
	}
	catch (const sol::error &e) {
		std::cerr << e.what() << std::endl;
	}
}

void ServerBlock::onBlockDestroyed(const glm::ivec3 &pos, World &world) const {
	try {
		if (m_onBlockDestroyed) {
			m_onBlockDestroyed(pos, world);
		}
	}
	catch (const sol::error &e) {
		std::cerr << e.what() << std::endl;
	}
}

