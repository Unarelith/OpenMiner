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
#include "NetworkController.hpp"
#include "ServerScene.hpp"

ServerScene::ServerScene(PlayerList &players) {
	m_controllers.emplace_back(new CollisionController(players));
	m_network = static_cast<NetworkController *>(m_controllers.emplace_back(new NetworkController).get());
}

void ServerScene::sendEntities(const ClientInfo &client) {
	m_network->sendEntities(m_registry, client);
}

void ServerScene::setServer(ServerCommandHandler *server) {
	m_network->setServer(server);
}

