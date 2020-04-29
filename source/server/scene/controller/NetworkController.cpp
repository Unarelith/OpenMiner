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
#include "AnimationComponent.hpp"
#include "DrawableDef.hpp"
#include "NetworkComponent.hpp"
#include "NetworkController.hpp"
#include "PositionComponent.hpp"
#include "RotationComponent.hpp"
#include "ServerCommandHandler.hpp"

void NetworkController::update(entt::registry &registry) {
	registry.view<NetworkComponent>().each([this] (auto, auto &network) {
		if (!network.hasSpawned) {
			m_server->sendEntitySpawn(network.entityID);
			network.hasSpawned = true;
		}
	});

	registry.view<NetworkComponent, PositionComponent>().each([this] (auto, auto &network, auto &position) {
		if (position.isUpdated) {
			m_server->sendEntityPosition(network.entityID, position.x, position.y, position.z);
			position.isUpdated = false;
		}
	});

	registry.view<NetworkComponent, RotationComponent>().each([this] (auto, auto &network, auto &rotation) {
		if (rotation.isUpdated) {
			m_server->sendEntityRotation(network.entityID, rotation.quat.w, rotation.quat.x, rotation.quat.y, rotation.quat.z);
			rotation.isUpdated = false;
		}
	});

	registry.view<NetworkComponent, AnimationComponent>().each([this] (auto, auto &network, auto &animation) {
		if (animation.isUpdated) {
			m_server->sendEntityAnimation(network.entityID, animation);
			animation.isUpdated = false;
		}
	});

	registry.view<NetworkComponent, DrawableDef>().each([this] (auto, auto &network, auto &drawableDef) {
		if (drawableDef.isUpdated) {
			m_server->sendEntityDrawableDef(network.entityID, drawableDef);
			drawableDef.isUpdated = false;
		}
	});
}

void NetworkController::sendEntities(entt::registry &registry, const ClientInfo &client) {
	registry.view<NetworkComponent>().each([&] (auto entity, auto &network) {
		m_server->sendEntitySpawn(network.entityID, &client);

		if (auto *position = registry.try_get<PositionComponent>(entity) ; position) {
			m_server->sendEntityPosition(network.entityID, position->x, position->y, position->z, &client);
		}

		if (auto *rotation = registry.try_get<RotationComponent>(entity) ; rotation) {
			m_server->sendEntityRotation(network.entityID, rotation->quat.w, rotation->quat.x, rotation->quat.y, rotation->quat.z, &client);
		}

		if (auto *animation = registry.try_get<AnimationComponent>(entity) ; animation) {
			m_server->sendEntityAnimation(network.entityID, *animation, &client);
		}

		if (auto *drawableDef = registry.try_get<DrawableDef>(entity) ; drawableDef) {
			m_server->sendEntityDrawableDef(network.entityID, *drawableDef, &client);
		}
	});
}

