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
#include <gk/core/Debug.hpp>

#include "LuaDimensionLoader.hpp"
#include "LuaMod.hpp"
#include "Registry.hpp"

void LuaDimensionLoader::loadDimension(const sol::table &table) const {
	std::string id = m_mod.id() + ":" + table["id"].get<std::string>();
	std::string name = table["name"].get<std::string>();

	sol::object biomesObject = table["biomes"];
	if (biomesObject.valid() && biomesObject.get_type() == sol::type::table) {
		Dimension &dimension = Registry::getInstance().registerDimension(id, name);
		dimension.setSky(table["sky"].get<std::string>());

		sol::object dimensionPhysicsObject = table["player_physics"];
		if (dimensionPhysicsObject.valid()) {
			if(dimensionPhysicsObject.get_type() == sol::type::table) {
				sol::table dimensionPhysicsTable = dimensionPhysicsObject.as<sol::table>();
				
				DimensionPhysics dimensionPhysics;
				
				dimensionPhysics.gravity = table["gravity"].get_or(1.f);
				dimensionPhysics.jumpSpeed = table["jump_speed"].get_or(0.05f);
				
				dimensionPhysics.jumpAntigravity = table["jump_antigravity"].get_or(0.3f);
				dimensionPhysics.glideGravity = table["glide_gravity"].get_or(0.04f);

				dimensionPhysics.horizontalSprintMod = table["horizontal_sprint_mod"].get_or(1.5f);
				dimensionPhysics.verticalSprintMod = table["vertical_sprint_mod"].get_or(1.5f);

				dimensionPhysics.moveSpeed = table["move_speed"].get_or(0.04f);
				dimensionPhysics.airSpeedMod = table["air_speed_mod"].get_or(0.75f);

				dimensionPhysics.flySpeed = table["fly_speed"].get_or(0.1f);
				dimensionPhysics.sneakVerticalSpeed = table["sneak_vertical_speed"].get_or(0.1f);
				dimensionPhysics.sneakHorizontalMod = table["sneak_horizontal_mod"].get_or(0.5f);

				dimensionPhysics.isSneakAlwaysMod = table["is_sneak_always_mod"].get_or(false);
				
				dimension.setPhysics(dimensionPhysics);
			}
			else
				gkError() << "For dimension" << name << ": Invalid player physics table";
		}
		
		sol::table biomesTable = biomesObject.as<sol::table>();
		for (auto &it : biomesTable) {
			if (it.second.get_type() == sol::type::string)
				dimension.addBiome(it.second.as<std::string>());
			else
				gkError() << "For dimension" << id << ": Invalid biome string";
		}
	}
	else
		gkError() << "For dimension" << id << ": Invalid biome table";
}

