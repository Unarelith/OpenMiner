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
#include "CollisionController.hpp"
#include "ItemStack.hpp"
#include "NetworkComponent.hpp"
#include "PlayerList.hpp"
#include "PositionComponent.hpp"
#include "ServerCommandHandler.hpp"

void CollisionController::update(entt::registry &registry) {
	// FIXME: This should be a callback in a CollisionComponent
	registry.view<PositionComponent, gk::DoubleBox, ItemStack, NetworkComponent>().each([&](auto entity, auto &position, auto &box, auto &itemStack, auto &network) {
		for (auto &it : m_players) {
			gk::DoubleBox hitbox = box + gk::Vector3d{position.x, position.y, position.z};
			gk::DoubleBox playerHitbox = it.second.hitbox() + gk::Vector3d{it.second.x(), it.second.y(), it.second.z()};
			if (hitbox.intersects(playerHitbox)) {
				it.second.inventory().addStack(itemStack.item().stringID(), itemStack.amount());
				m_server->sendPlayerInvUpdate(it.second.clientID(), &it.second.client());
				m_server->sendEntityDespawn(network.entityID);
				registry.destroy(entity);
			}
		}
	});
}

