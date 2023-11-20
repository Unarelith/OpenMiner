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
#include "common/world/Chunk.hpp"
#include "common/world/Player.hpp"
#include "common/world/World.hpp"
#include "server/network/ServerCommandHandler.hpp"
#include "server/world/ServerBlock.hpp"
#include "server/world/ServerPlayer.hpp"
#include "server/world/ServerWorld.hpp"

void ServerBlock::onTick(const glm::ivec3 &pos, ServerChunk &chunk, ServerWorld &world, ServerCommandHandler &server) const {
	if (m_onTickEnabled && m_onTick) {
		try {
			m_onTick(pos, *this, chunk, world);

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
		catch (const sol::error &e) {
			m_onTickEnabled = false;
			logError() << e.what();
			logError() << "Block stopped ticking at (" << pos.x << ", " << pos.y << ", " << pos.z << ")";
		}
	}
}

bool ServerBlock::onBlockActivated(const glm::ivec3 &pos, ServerPlayer &player, ServerWorld &world, ClientInfo &client, ServerCommandHandler &server) const {
	try {
		if (m_onBlockActivated) {
			m_onBlockActivated(pos, *this, player, world, client, server);

			// FIXME: Check if data changed before sending
			BlockData *blockData = world.getBlockData(pos.x, pos.y, pos.z);
			if (blockData)
				server.sendBlockDataUpdate(pos.x, pos.y, pos.z, blockData);

			return true;
		}
	}
	catch (const sol::error &e) {
		logError() << e.what();
	}

	return false;
}

void ServerBlock::onBlockPlaced(const glm::ivec3 &pos, ServerWorld &world) const {
	try {
		if (m_onBlockPlaced) {
			m_onBlockPlaced(pos, world);
		}
	}
	catch (const sol::error &e) {
		logError() << e.what();
	}
}

void ServerBlock::onBlockDestroyed(const glm::ivec3 &pos, ServerWorld &world) const {
	try {
		if (m_onBlockDestroyed) {
			m_onBlockDestroyed(pos, world);
		}
	}
	catch (const sol::error &e) {
		logError() << e.what();
	}
}

// Please update 'docs/lua-api-cpp.md' if you change this
void ServerBlock::initUsertype(sol::state &lua) {
	lua.new_usertype<ServerBlock>("ServerBlock",
		sol::base_classes, sol::bases<Block>()
	);
}
