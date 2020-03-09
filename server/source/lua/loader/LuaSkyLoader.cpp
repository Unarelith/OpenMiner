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
#include "LuaMod.hpp"
#include "LuaSkyLoader.hpp"
#include "Registry.hpp"

void LuaSkyLoader::loadSky(const sol::table &table) const {
	std::string stringID = m_mod.id() + ":" + table["id"].get<std::string>();

	Sky &sky = Registry::getInstance().registerSky(stringID);

	const sol::table &skyColor = table["color"].get<sol::table>();
	if (skyColor.valid()) {
		u8 r = skyColor["day"][1].get<u8>();
		u8 g = skyColor["day"][2].get<u8>();
		u8 b = skyColor["day"][3].get<u8>();
		u8 a = skyColor["day"][4].get_or<u8>(255);
		sky.setColor(gk::Color{r, g, b, a});
	}

	const sol::table &fogColor = table["fog_color"].get<sol::table>();
	if (fogColor.valid()) {
		u8 r = fogColor["day"][1].get<u8>();
		u8 g = fogColor["day"][2].get<u8>();
		u8 b = fogColor["day"][3].get<u8>();
		u8 a = fogColor["day"][4].get_or<u8>(255);
		sky.setFogColor(gk::Color{r, g, b, a});
	}
}

