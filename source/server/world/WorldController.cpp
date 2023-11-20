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
#include "common/core/Registry.hpp"
#include "server/world/WorldController.hpp"
#include "server/world/save/WorldSaveBasicBackend.hpp"

void WorldController::init(PlayerList &players, s32 seed) {
	for (const Dimension &dimension : m_registry.dimensions()) {
		m_worldList.emplace_back(players, dimension, seed);
		m_worldList.back().setServer(m_server);
	}

	m_worldSaveBackend.reset(new WorldSaveBasicBackend{m_worldList, players});
}

void WorldController::clearEntities() {
	for (auto &it : m_worldList)
		it.scene().clear();
}

void WorldController::update(bool doTick) {
	for (auto &it : m_worldList)
		it.update(doTick);
}

ServerWorld &WorldController::getWorld(u16 dimension) {
	if (m_worldList.size() <= dimension)
		throw EXCEPTION("Failed to get world object for dimension", dimension);

	return m_worldList.at(dimension);
}
