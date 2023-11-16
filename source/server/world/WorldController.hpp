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

#include "server/world/ServerWorld.hpp"
#include "server/world/save/WorldSaveBackend.hpp"

class Registry;

class WorldController {
	public:
		WorldController(Registry &registry) : m_registry(registry) {}

		void init(PlayerList &players, s32 seed);

		void clearEntities();

		void update(bool doTick);

		void load(const std::string &worldName) { m_worldSaveBackend->load(worldName); }
		void save(const std::string &worldName) { m_worldSaveBackend->save(worldName); }

		ServerWorld &getWorld(u16 dimension);

		const ServerCommandHandler *server() const { return m_server; }
		void setServer(ServerCommandHandler &server) { m_server = &server; }

	private:
		std::deque<ServerWorld> m_worldList;

		Registry &m_registry;

		ServerCommandHandler *m_server = nullptr;

		std::unique_ptr<WorldSaveBackend> m_worldSaveBackend{nullptr};
};

#endif // WORLDCONTROLLER_HPP_
