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

	if (sol::object obj = table["daylight_cycle"] ; obj.valid()) {
		sol::table daylightCycleTable = obj.as<sol::table>();
		sky.setDaylightCycleSpeed(daylightCycleTable["speed"].get_or(0.f));
	}

	loadObjects(sky, table);
}

void LuaSkyLoader::loadObjects(Sky &sky, const sol::table &table) const {
	if (sol::object obj = table["objects"] ; obj.valid()) {
		sol::table objectsTable = obj.as<sol::table>();

		if (sol::object obj = objectsTable["sun"] ; obj.valid()) {
			sol::table sunTable = obj.as<sol::table>();

			Sky::SunDefinition sunDefinition;
			sunDefinition.texture = sunTable["texture"].get_or<std::string>("");
			sunDefinition.size = sunTable["size"].get_or(256);

			sky.setSunDefinition(sunDefinition);
		}

		if (sol::object obj = objectsTable["moon"] ; obj.valid()) {
			sol::table moonTable = obj.as<sol::table>();

			Sky::MoonDefinition moonDefinition;
			moonDefinition.texture = moonTable["texture"].get_or<std::string>("");
			moonDefinition.size = moonTable["size"].get_or(256);

			if (sol::object obj = moonTable["phases"] ; obj.valid()) {
				sol::table phasesTable = obj.as<sol::table>();
				moonDefinition.phaseCount = phasesTable["count"].get_or(1);
				moonDefinition.phaseSize = phasesTable["size"].get_or(32);
			}

			sky.setMoonDefinition(moonDefinition);
		}

		if (sol::object obj = objectsTable["stars"] ; obj.valid()) {
			sol::table starsTable = obj.as<sol::table>();

			Sky::StarsDefinition starsDefinition;
			starsDefinition.count = starsTable["count"].get_or(1000);
			starsDefinition.size = starsTable["size"].get_or(4);

			sky.setStarsDefinition(starsDefinition);
		}
	}
}

