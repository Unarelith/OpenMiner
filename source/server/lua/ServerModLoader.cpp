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

struct ModEntry {
	fs::path path;
	std::string id;

	std::vector<std::string> dependencies;
	std::vector<ModEntry *> requiredBy;

	bool isLoaded = false;
	bool isValid = true;
};

void ServerModLoader::loadMods() {
	std::unordered_map<std::string, ModEntry> mods;
	sol::state lua;

	fs::path basePath = fs::current_path();
	fs::directory_iterator dir("mods/");
	for (const auto &entry : dir) {
		if (fs::exists(entry.path().string() + "/config.lua")) {
			try {
				lua.safe_script_file(entry.path().string() + "/config.lua");

				sol::table config = lua["mod_config"].get<sol::table>();

				ModEntry mod;
				mod.path = entry.path().string();
				mod.id = config["id"];

				if (!mod.id.empty()) {
					if (mod.id != entry.path().filename().string())
						gkWarning() << ("Mod ID '" + mod.id + "' is different from folder name '" + entry.path().filename().string() + "'").c_str();

					auto it = mods.find(mod.id);
					if (it == mods.end()) {
						sol::object dependencies = config["dependencies"];
						if (dependencies.get_type() == sol::type::table) {
							for (auto &it : dependencies.as<sol::table>()) {
								mod.dependencies.emplace_back(it.second.as<std::string>());
							}
						}
						else
							gkWarning() << ("Failed to load dependencies for mod '" + mod.id + "'").c_str();

						mods.emplace(mod.id, mod);
					}
					else {
						gkError() << ("Trying to load mod '" + mod.id + "' twice").c_str();
						gkError() << "Loaded path: " << it->second.path.string();
						gkError() << "Current path:" << mod.path.string();
					}
				}
				else
					gkError() << ("Failed to load mod '" + mod.path.filename().string() + "': Mod ID required in 'config.lua'").c_str();
			}
			catch (sol::error &e) {
				gkError() << e.what();
			}

		}
		else
			gkError() << ("The mod at '" + entry.path().string() + "' doesn't contain a 'config.lua' file.").c_str();
	}

	std::queue<ModEntry *> dependencyTree;

	// Small BFS to check cyclic dependencies and build the dependency tree
	for (auto &modit : mods) {
		std::queue<ModEntry *> queue;
		queue.emplace(&modit.second);
		while (!queue.empty()) {
			ModEntry *mod = queue.front();
			queue.pop();

			if (mod->dependencies.empty())
				dependencyTree.emplace(mod);

			for (const std::string &dependencyID : mod->dependencies) {
				if (dependencyID == modit.second.id) {
					gkError() << ("Cyclic dependency detected for mod '" + modit.second.id + "' in mod '" + mod->id + "'").c_str();
					mod->isValid = false;
					break;
				}

				auto it = mods.find(dependencyID);
				if (it != mods.end()) {
					it->second.requiredBy.emplace_back(mod);
					queue.emplace(&it->second);
				}
				else {
					gkError() << ("Mod '" + mod->id + "' cannot be loaded: Missing dependency '" + dependencyID + "'").c_str();
					mod->isValid = false;
				}
			}
		}
	}

	// TODO: Two types of dependencies should be handled
	//       - Required (cyclic depedencies not handled)
	//       - Optional (cyclic dependencies handled)

	m_scriptEngine.init();
	m_scriptEngine.luaCore().setModLoader(this);

	while (!dependencyTree.empty()) {
		ModEntry *mod = dependencyTree.front();
		dependencyTree.pop();

		if (mod->isLoaded || !mod->isValid) continue;

		if (fs::exists(mod->path.string() + "/init.lua")) {
			fs::current_path(mod->path.string());

			try {
				m_scriptEngine.lua().safe_script_file("init.lua");
			}
			catch (const sol::error &e) {
				gkError() << "Error: Failed to load mod at" << mod->path.string();
				gkError() << e.what();
			}

			fs::current_path(basePath);

			gkInfo() << "Mod" << mod->id << "loaded";
		}
		else
			gkError() << ("The mod at '" + mod->path.string() + "' doesn't contain an 'init.lua' file.").c_str();

		mod->isLoaded = true;

		for (ModEntry *entry : mod->requiredBy) {
			dependencyTree.emplace(entry);
		}
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

// Please update 'docs/lua-api-cpp.md' if you change this
void ServerModLoader::initUsertype(sol::state &lua) {
	lua.new_usertype<ServerModLoader>("ServerModLoader",
		"register_mod", &ServerModLoader::registerMod
	);
}

