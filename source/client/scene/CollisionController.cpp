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
#include "ClientPlayer.hpp"
#include "CollisionController.hpp"
#include "InventoryCube.hpp"
#include "ItemStack.hpp"

void CollisionController::update(entt::DefaultRegistry &registry, ClientPlayer &player) {
	// FIXME: This shouldn't use InventoryCube, but instead a callback stored in a CollisionComponent
	registry.view<InventoryCube, gk::DoubleBox, ItemStack>().each([&](auto entity, auto &cube, auto &box, auto &itemStack) {
		gk::DoubleBox hitbox = box + cube.getPosition();
		gk::DoubleBox playerHitbox = player.hitbox() + gk::Vector3d{player.x(), player.y(), player.z()};
		if (hitbox.intersects(playerHitbox)) {
			player.inventory().addStack(itemStack.item().stringID(), itemStack.amount());
			registry.destroy(entity);
		}
	});
}

