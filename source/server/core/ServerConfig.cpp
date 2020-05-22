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
#include "ServerConfig.hpp"

// Gameplay
bool ServerConfig::useItemDrops = false;

#include <gk/core/Debug.hpp>
#include <gk/core/Filesystem.hpp>

#include <sol/sol.hpp>

void ServerConfig::loadConfigFromFile(const char *file) {
	if (gk::Filesystem::fileExists(file)) {
		sol::state lua;

		try {
			lua.safe_script_file(file);

			useItemDrops = lua["useItemDrops"].get_or(useItemDrops);

			gkInfo() << "Config file loaded successfully";
		}
		catch (sol::error &e) {
			gkError() << e.what();
		}
	}
}

