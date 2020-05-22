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
#ifndef ENTITYWRAPPER_HPP_
#define ENTITYWRAPPER_HPP_

#include <entt/entt.hpp>

#include "ItemStack.hpp"
#include "NetworkComponent.hpp"
#include "PositionComponent.hpp"

// This class is meant to be used ONLY in Lua or Lua-related C++ classes
class EntityWrapper {
	public:
		EntityWrapper(entt::entity id, entt::registry &registry)
			: m_id(id), m_registry(registry) {}

		entt::entity id() const { return m_id; }

		PositionComponent *getPositionComponent() const { return m_registry.try_get<PositionComponent>(m_id); }
		NetworkComponent *getNetworkComponent() const { return m_registry.try_get<NetworkComponent>(m_id); }
		ItemStack *getItemStack() const { return m_registry.try_get<ItemStack>(m_id); }

		static void initUsertype(sol::state &lua);

	private:
		entt::entity m_id;
		entt::registry &m_registry;
};

#endif // ENTITYWRAPPER_HPP_
