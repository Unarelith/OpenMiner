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
#include "Network.hpp"
#include "NetworkComponent.hpp"
#include "NetworkController.hpp"
#include "Server.hpp"
#include "ServerCommandHandler.hpp"

void NetworkController::update(entt::registry &registry) {
	registry.view<NetworkComponent>().each([&] (auto entity, auto &network) {
		if (!network.hasSpawned) {
			m_server->sendEntitySpawn(network.entityID);
			network.hasSpawned = true;
		}

		registry.visit(entity, [&] (const auto &component_type) {
			const auto &type = entt::resolve_type(component_type);
			const auto &component = type.func("get"_hs).invoke({}, std::ref(registry), entity);
			Network::Packet packet = type.func("serialize"_hs).invoke({}, entity, component, true).template cast<Network::Packet>();
			if (packet.getDataSize())
				m_server->server().sendToAllClients(packet);
		});
	});
}

void NetworkController::sendEntities(entt::registry &registry, const ClientInfo &client) {
	registry.view<NetworkComponent>().each([&] (auto entity, auto &network) {
		m_server->sendEntitySpawn(network.entityID, &client);

		// gkDebug() << "sendEntitySpawn" << std::underlying_type_t<entt::entity>(network.entityID);

		registry.visit(entity, [&] (const auto &component_type) {
			const auto &type = entt::resolve_type(component_type);
			if (type.prop("is_serializable"_hs).value().template cast<bool>()) {
				const auto &component = type.func("get"_hs).invoke({}, std::ref(registry), entity);
				Network::Packet packet = type.func("serialize"_hs).invoke({}, entity, component, false).template cast<Network::Packet>();
				if (packet.getDataSize()) {
					// gkDebug() << "sendEntities: Serializing component" << type.prop("name"_hs).value().template cast<std::string>() << "for entity" << std::underlying_type_t<entt::entity>(entity) << "of size" << packet.getDataSize();
					client.tcpSocket->send(packet);
				}
			}
		});
	});
}

