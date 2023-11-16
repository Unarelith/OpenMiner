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
#ifndef COLLISIONCONTROLLER_HPP_
#define COLLISIONCONTROLLER_HPP_

#include "common/scene/controller/AbstractController.hpp"

class PlayerList;
class ServerCommandHandler;

class CollisionController : public AbstractController {
	public:
		CollisionController(PlayerList &players) : m_players(players) {}

		void update(entt::registry &registry);

		void setServer(ServerCommandHandler *server) { m_server = server; }

	private:
		ServerCommandHandler *m_server = nullptr;

		PlayerList &m_players;
};

#endif // COLLISIONCONTROLLER_HPP_
