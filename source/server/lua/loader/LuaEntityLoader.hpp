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
#ifndef LUAENTITYLOADER_HPP_
#define LUAENTITYLOADER_HPP_

#include <sol/sol.hpp>

#include <entt/entt.hpp>

#include "common/inventory/ItemStack.hpp"

class LuaMod;
class WorldController;

class LuaEntityLoader {
	public:
		LuaEntityLoader(LuaMod &mod, WorldController &worldController)
			: m_mod(mod), m_worldController(worldController) {}

		void loadEntity(const sol::table &table) const;
		void spawnEntity(const std::string &entityID, const sol::table &table) const;

	private:
		void tryLoadCallbacks(const sol::table &table) const;
		void tryLoadVisual(const sol::table &table, entt::registry &registry, entt::entity entity) const;
		void tryLoadRotation(const sol::table &table, entt::registry &registry, entt::entity entity) const;
		void tryLoadAnimation(const sol::table &table, entt::registry &registry, entt::entity entity) const;
		void tryLoadHitbox(const sol::table &table, entt::registry &registry, entt::entity entity) const;
		ItemStack *tryLoadItemStack(const sol::table &table, entt::registry &registry, entt::entity entity) const;

		LuaMod &m_mod;
		WorldController &m_worldController;

		mutable std::string m_stringID;
};

#endif // LUAENTITYLOADER_HPP_
