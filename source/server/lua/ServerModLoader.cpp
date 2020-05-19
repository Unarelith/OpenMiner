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
#include <gk/core/Exception.hpp>
#include <gk/core/Utils.hpp>

#include <filesystem.hpp>

#include "LuaMod.hpp"
#include "ScriptEngine.hpp"
#include "ServerModLoader.hpp"

namespace fs = ghc::filesystem;

void ServerModLoader::loadMods() {
	m_scriptEngine.init();
	m_scriptEngine.luaCore().setModLoader(this);

	fs::path basePath = fs::current_path();
	fs::directory_iterator dir("mods/");
	for (const auto &entry : dir) {
		if (fs::exists(entry.path().string() + "/init.lua")) {
			fs::current_path(entry.path().string());

			try {
				m_scriptEngine.lua().safe_script_file("init.lua");
			}
			catch (const sol::error &e) {
				std::cerr << "Error: Failed to load mod at '" << entry.path().string() << "'" << std::endl;
				std::cerr << e.what() << std::endl;
			}

			fs::current_path(basePath);

			gkInfo() << "Mod" << entry.path().filename().string() << "loaded";
		}
		else
			gkWarning() << "The mod" << entry.path().filename().string() << "doesn't contain an 'init.lua' file.";
	}

	for (auto &it : m_mods) {
		// gkDebug() << "Applying mod" << it.second.id() << "...";
		it.second.commit();
	}
}

LuaMod &ServerModLoader::registerMod(const std::string &name) {
	// gkDebug("Registering mod" << mod.id() << "...";

	if (!gk::regexMatch(name, "^[A-Za-z0-9_]+$") || name == "group")
		throw std::runtime_error("Mod name '" + name + "' is invalid.");

	auto it = m_mods.find(name);
	if (it != m_mods.end())
		throw std::runtime_error("The mod '" + name + "' has already been loaded. Mod name must be unique.");

	m_mods.emplace(
		std::piecewise_construct,
		std::forward_as_tuple(name),
		std::forward_as_tuple(name, m_registry, m_worldController, m_players)
	);

	return m_mods.at(name);
}

