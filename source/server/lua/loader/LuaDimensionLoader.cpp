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
#include "Debug.hpp"
#include "LuaDimensionLoader.hpp"
#include "LuaMod.hpp"
#include "Registry.hpp"

void LuaDimensionLoader::loadDimension(const sol::table &table) const {
	std::string id = m_mod.id() + ":" + table["id"].get<std::string>();
	std::string name = table["name"].get<std::string>();

	sol::object biomesObject = table["biomes"].get<sol::object>();
	if (biomesObject.valid() && biomesObject.get_type() == sol::type::table) {
		Dimension &dimension = Registry::getInstance().registerDimension(id, name);
		dimension.setSky(table["sky"].get<std::string>());
		dimension.setGravity(table["gravity"].get_or(1.f));

		sol::table biomesTable = biomesObject.as<sol::table>();
		for (auto &it : biomesTable) {
			if (it.second.get_type() == sol::type::string)
				dimension.addBiome(it.second.as<std::string>());
			else
				logError() << "For dimension" << id << ": Invalid biome string";
		}
	}
	else
		logError() << "For dimension" << id << ": Invalid biome table";
}

