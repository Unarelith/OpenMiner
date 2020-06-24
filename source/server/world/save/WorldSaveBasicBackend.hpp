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
#ifndef WORLDSAVEBASICBACKEND_HPP_
#define WORLDSAVEBASICBACKEND_HPP_

#include <deque>
#include <unordered_map>

#include <entt/entt.hpp>

#include "WorldSaveBackend.hpp"

namespace sf { class Packet; }

class ServerWorld;

class WorldSaveBasicBackend : public WorldSaveBackend {
	public:
		WorldSaveBasicBackend(std::deque<ServerWorld> &worldList)
			: m_worldList(worldList) {}

		void load(const std::string &worldName) override;
		void save(const std::string &worldName) override;

	private:
		void loadEntities(sf::Packet &save, ServerWorld &world);
		void saveEntities(sf::Packet &save, ServerWorld &world);

		std::deque<ServerWorld> &m_worldList;

		std::unordered_map<entt::entity, entt::entity> m_entityMap;
};

#endif // WORLDSAVEBASICBACKEND_HPP_
