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
#ifndef SERVERMODLOADER_HPP_
#define SERVERMODLOADER_HPP_

#include <string>
#include <unordered_map>

#include "LuaMod.hpp"

class PlayerList;
class Registry;
class ScriptEngine;
class WorldController;

class ServerModLoader {
	public:
		ServerModLoader(ScriptEngine &scriptEngine, Registry &registry, WorldController &worldController, PlayerList &players)
			: m_scriptEngine(scriptEngine), m_registry(registry), m_worldController(worldController), m_players(players) {}

		void loadMods();

		LuaMod &registerMod(const std::string &name);

	private:
		ScriptEngine &m_scriptEngine;
		Registry &m_registry;
		WorldController &m_worldController;
		PlayerList &m_players;

		std::unordered_map<std::string, LuaMod> m_mods;
};

#endif // SERVERMODLOADER_HPP_
