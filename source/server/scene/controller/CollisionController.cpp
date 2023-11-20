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
#include "common/core/Debug.hpp"
#include "common/core/Registry.hpp"
#include "common/inventory/ItemStack.hpp"
#include "common/scene/component/NetworkComponent.hpp"
#include "common/scene/component/PositionComponent.hpp"
#include "server/core/PlayerList.hpp"
#include "server/network/ServerCommandHandler.hpp"
#include "server/scene/EntityWrapper.hpp"
#include "server/scene/controller/CollisionController.hpp"

void CollisionController::update(entt::registry &registry) {
	registry.view<PositionComponent, DoubleBox, std::string>().each([&](auto entity, auto &position, auto &box, auto &id) {
		for (auto &it : m_players) {
			if (it.second.dimension() == position.dimension) {
				DoubleBox hitbox = box + Vector3d{position.x, position.y, position.z};
				DoubleBox playerHitbox = it.second.hitbox() + Vector3d{it.second.x(), it.second.y(), it.second.z()};
				if (hitbox.intersects(playerHitbox)) {
					EntityWrapper entityWrapper{entity, registry};

					auto &entityCallbackContainer = Registry::getInstance().getEntityCallbackContainer(id);
					entityCallbackContainer.collisionCallback(entityWrapper, it.second, *m_server);
				}
			}
		}
	});
}
