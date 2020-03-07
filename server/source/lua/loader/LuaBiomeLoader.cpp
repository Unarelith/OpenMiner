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
#include "LuaBiomeLoader.hpp"
#include "LuaMod.hpp"
#include "Registry.hpp"

void LuaBiomeLoader::loadTree(const sol::table &table) const {
	std::string stringID = m_mod.id() + ":" + table["id"].get<std::string>();
	std::string label = table["name"].get<std::string>();

	Tree &tree = Registry::getInstance().registerTree(stringID, label);
	tree.setLogBlockID(Registry::getInstance().getBlockFromStringID(table["log_block"]).id());
	tree.setLeavesBlockID(Registry::getInstance().getBlockFromStringID(table["leaves_block"]).id());
}

void LuaBiomeLoader::loadBiome(const sol::table &table) const {
	std::string stringID = m_mod.id() + ":" + table["id"].get<std::string>();
	std::string label = table["name"].get<std::string>();

	Biome &biome = Registry::getInstance().registerBiome(stringID, label);

	loadBiomeParameters(biome, table);
	loadBiomeBlocks(biome, table);

	loadTreePlacementEntries(biome, table);
	loadFloraPlacementEntries(biome, table);
	loadOrePlacementEntries(biome, table);
}

inline void LuaBiomeLoader::loadBiomeParameters(Biome &biome, const sol::table &table) const {
	// TODO eventually a WorldType could have a list of biome parameter names in order,
	// and we could use those as the ordered keys.
	// Currently hardcoding "temperature" and "precipitation" to get something functional.
	biome.addParameter(table["params"]["temperature"]);
	biome.addParameter(table["params"]["precipitation"]);
}

inline void LuaBiomeLoader::loadBiomeBlocks(Biome &biome, const sol::table &table) const {
	biome.setTopBlockID(Registry::getInstance().getBlockFromStringID(table["top_block"]).id());
	biome.setGroundBlockID(Registry::getInstance().getBlockFromStringID(table["ground_block"]).id());
	biome.setDeepBlockID(Registry::getInstance().getBlockFromStringID(table["deep_block"]).id());
	biome.setBeachBlockID(Registry::getInstance().getBlockFromStringID(table["beach_block"]).id());
	biome.setLiquidBlockID(Registry::getInstance().getBlockFromStringID(table["liquid_block"]).id());
}

inline void LuaBiomeLoader::loadTreePlacementEntries(Biome &biome, const sol::table &table) const {
	sol::optional<sol::table> treeDefinitions = table["trees"];
	if (treeDefinitions != sol::nullopt) {
		for (const auto &it : treeDefinitions.value()) {
			sol::table treeDefinition = it.second.as<sol::table>();

			PlacementEntry::Tree &treeEntry = biome.addTree();
			treeEntry.treeID = Registry::getInstance().getTreeFromStringID(treeDefinition["type"]).id();
			treeEntry.probability = treeDefinition["probability"];
		}
	}
}

inline void LuaBiomeLoader::loadFloraPlacementEntries(Biome &biome, const sol::table &table) const {
	sol::optional<sol::table> floraDefinitions = table["flora"];
	if (floraDefinitions != sol::nullopt) {
		for (const auto &it : floraDefinitions.value()) {
			sol::table floraDefinition = it.second.as<sol::table>();

			PlacementEntry::Flora &floraEntry = biome.addFlora();
			floraEntry.blockID = Registry::getInstance().getBlockFromStringID(floraDefinition["block"]).id();
			floraEntry.spawnsOnBlockID = Registry::getInstance().getBlockFromStringID(floraDefinition["spawns_on"]).id();
			floraEntry.probability = floraDefinition["probability"];
		}
	}
}

inline void LuaBiomeLoader::loadOrePlacementEntries(Biome &biome, const sol::table &table) const {
	sol::optional<sol::table> oreDefinitions = table["ore"];
	if (oreDefinitions != sol::nullopt) {
		for (const auto &it : oreDefinitions.value()) {
			sol::table oreDefinition = it.second.as<sol::table>();

			PlacementEntry::Ore &oreEntry = biome.addOre();
			oreEntry.blockID = Registry::getInstance().getBlockFromStringID(oreDefinition["block"]).id();
			oreEntry.probability = oreDefinition["probability"];
			oreEntry.size = oreDefinition["size"];
		}
	}
}

