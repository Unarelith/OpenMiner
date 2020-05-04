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
#ifndef LUACORE_HPP_
#define LUACORE_HPP_

#include <functional>
#include <iostream>

#include <sol.hpp>

class Registry;
class ServerModLoader;

enum class LuaEventType {
	OnBlockPlaced,
	OnBlockDigged,
	OnBlockActivated
};

class LuaCore {
	public:
		void addListener(LuaEventType eventType, const sol::function &listener);

		template<typename... Args>
		void onEvent(LuaEventType eventType, Args &&...args) {
			for (auto &it : m_listeners) {
				if (it.first == eventType) {
					try {
						it.second(std::forward<Args>(args)...);
					}
					catch (const sol::error &e) {
						std::cerr << e.what() << std::endl;
					}
				}
			}
		}

		void setRegistry(Registry *registry) { m_registry = registry; }
		void setModLoader(ServerModLoader *modLoader) { m_modLoader = modLoader; }

		static void initUsertype(sol::state &lua);

	private:
		Registry *m_registry = nullptr;
		ServerModLoader *m_modLoader = nullptr;

		std::unordered_multimap<LuaEventType, sol::unsafe_function> m_listeners;
};

#endif // LUACORE_HPP_
