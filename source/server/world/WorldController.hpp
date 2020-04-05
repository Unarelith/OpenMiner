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
#ifndef WORLDCONTROLLER_HPP_
#define WORLDCONTROLLER_HPP_

#include <deque>

#include "ServerWorld.hpp"

namespace gk {
	class GameClock;
}

class Registry;

class WorldController {
	public:
		WorldController(Registry &registry, gk::GameClock &clock) : m_registry(registry), m_clock(clock) {}

		void init(PlayerList &players);

		void update();

		void load(const std::string &name);
		void save(const std::string &name);

		ServerWorld &getWorld(u16 dimension) { return m_worldList.at(dimension); }

		void setServer(ServerCommandHandler &server) { m_server = &server; }

	private:
		std::vector<ServerWorld> m_worldList;

		Registry &m_registry;

		gk::GameClock &m_clock;

		ServerCommandHandler *m_server = nullptr;
};

#endif // WORLDCONTROLLER_HPP_
