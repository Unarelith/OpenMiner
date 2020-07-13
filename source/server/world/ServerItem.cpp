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
#include <gk/core/Debug.hpp>

#include "Chunk.hpp"
#include "Player.hpp"
#include "ServerCommandHandler.hpp"
#include "ServerItem.hpp"
#include "ServerPlayer.hpp"
#include "World.hpp"

bool ServerItem::onItemActivated(const glm::ivec3 &pos, Block &block, Player &player, World &world, ClientInfo &client, ServerCommandHandler &server) const {
	try {
		if (m_onItemActivated) {
			m_onItemActivated(pos, block, player, world, client, server);
			return true;
		}
	}
	catch (const sol::error &e) {
		gkError() << e.what();
	}

	return false;
}

void ServerItem::setOnItemActivated(const sol::protected_function &function) {
	m_onItemActivated = function;
	m_canBeActivated = m_onItemActivated.valid();
}

