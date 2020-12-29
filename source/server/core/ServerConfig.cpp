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
#include <fstream>

#include <gk/core/Debug.hpp>
#include <gk/core/Filesystem.hpp>

#include "ServerConfig.hpp"

// Server
u8 ServerConfig::maxPlayers = 5;
u16 ServerConfig::maxItemStackSize = 64;

// World
u8 ServerConfig::renderDistance = 1;

// Mod-defined options
std::unordered_map<std::string, sol::object> ServerConfig::options;

static sol::state lua;

void ServerConfig::loadConfigFromFile(const char *file) {
	if (gk::Filesystem::fileExists(file)) {
		try {
			lua.safe_script_file(file);

			maxPlayers = lua["max_players"].get_or(maxPlayers);
			maxItemStackSize = lua["max_item_stack_size"].get_or(maxItemStackSize);

			renderDistance = lua["render_distance"].get_or(renderDistance);

			if (lua["mod_options"].valid() && lua["mod_options"].get_type() == sol::type::table) {
				for (auto &it : lua["mod_options"].get<sol::table>()) {
					options.emplace(it.first.as<std::string>(), it.second);
				}
			}

			gkInfo() << "Config file loaded successfully";
		}
		catch (sol::error &e) {
			gkError() << e.what();
		}
	}
}

void ServerConfig::saveConfigToFile(const char *filename) {
	std::ofstream file{filename, std::ofstream::out | std::ofstream::trunc};
	file << "max_players = " << (u16)maxPlayers << std::endl;
	file << "max_item_stack_size = " << maxItemStackSize << std::endl;
	file << "render_distance = " << (u16)renderDistance << std::endl;
	file << "mod_options = {" << std::endl;

	for (auto &it : options) {
		file << "\t[\"" << it.first << "\"] = ";

		if (it.second.get_type() == sol::type::boolean) {
			bool value = it.second.as<bool>();
			file << (value ? "true" : "false");
		}
		else if (it.second.get_type() == sol::type::number) {
			file << it.second.as<double>();
		}
		else if (it.second.get_type() == sol::type::string) {
			file << it.second.as<std::string>();
		}
		else {
			file << "nil";
		}

		file << "," << std::endl;
	}

	file << "}" << std::endl;
}

bool ServerConfig::assignOption(const std::string &name, const std::string &value) {
	auto it = options.find(name);
	if (it != options.end()) {
		try {
			sol::object object = lua.load("return " + value)();
			if (object.valid()
				&& (object.get_type() == sol::type::boolean
				 || object.get_type() == sol::type::number
				 || object.get_type() == sol::type::string)) {
				it->second = object;
				return true;
			}
		}
		catch (sol::error &e) {
			gkWarning() << e.what();
		}
	}
	else {
		gkWarning() << "Can't assign option: '" + name + "' doesn't exist";
	}

	return false;
}

