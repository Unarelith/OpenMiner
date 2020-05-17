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

#include "CraftingRecipe.hpp"
#include "SmeltingRecipe.hpp"
#include "Registry.hpp"

Registry *Registry::s_instance = nullptr;

Item &Registry::registerItem(const TilesDef &tiles, const std::string &stringID, const std::string &label) {
	u32 id = m_items.size();
	m_itemsID.emplace(stringID, id);
	m_items.emplace_back(id, tiles, stringID, label);
	return m_items.back();
}

Item &Registry::registerSerializedItem(sf::Packet &packet) {
	m_items.emplace_back();
	m_items.back().deserialize(packet);

	u32 id = m_items.size() - 1;
	m_itemsID.emplace(m_items.back().stringID(), id);

	return m_items.back();
}

Sky &Registry::registerSky(const std::string &stringID) {
	size_t id = m_skies.size();
	m_skiesID.emplace(stringID, id);
	m_skies.emplace_back(id, stringID);
	return m_skies.back();
}

Sky &Registry::registerSerializedSky(sf::Packet &packet) {
	m_skies.emplace_back();
	m_skies.back().deserialize(packet);

	size_t id = m_skies.size() - 1;
	m_skiesID.emplace(m_skies.back().stringID(), id);

	return m_skies.back();
}

Tree &Registry::registerTree(const std::string &stringID) {
	size_t id = m_trees.size();
	m_treesID.emplace(stringID, id);
	m_trees.emplace_back(id, stringID);
	return m_trees.back();
}

Tree &Registry::registerSerializedTree(sf::Packet &packet) {
	m_trees.emplace_back();
	m_trees.back().deserialize(packet);

	size_t id = m_trees.size() - 1;
	m_treesID.emplace(m_trees.back().stringID(), id);

	return m_trees.back();
}

Biome &Registry::registerBiome(const std::string &stringID, const std::string &label) {
	size_t id = m_biomes.size();
	m_biomesID.emplace(stringID, id);
	m_biomes.emplace_back(id, stringID, label);
	return m_biomes.back();
}

Biome &Registry::registerSerializedBiome(sf::Packet &packet) {
	m_biomes.emplace_back();
	m_biomes.back().deserialize(packet);

	size_t id = m_biomes.size() - 1;
	m_biomesID.emplace(m_biomes.back().stringID(), id);

	return m_biomes.back();
}

Dimension &Registry::registerDimension(const std::string &stringID, const std::string &label) {
	u16 id = m_dimensions.size();
	m_dimensionsID.emplace(stringID, id);
	m_dimensions.emplace_back(id, stringID, label);
	return m_dimensions.back();
}

Dimension &Registry::registerSerializedDimension(sf::Packet &packet) {
	m_dimensions.emplace_back();
	m_dimensions.back().deserialize(packet);

	u16 id = m_dimensions.size() - 1;
	m_dimensionsID.emplace(m_biomes.back().stringID(), id);

	return m_dimensions.back();
}

entt::entity Registry::registerEntity(const std::string &stringID) {
	auto it = m_entities.find(stringID);
	if (it == m_entities.end()) {
		entt::entity entity = m_entityRegistry.create();
		m_entities.emplace(stringID, entity);
		return entity;
	}
	else
		gkError() << "Redefinition of entity '" + stringID + "', keeping the first one";

	return entt::null;
}

const Block &Registry::getBlockFromStringID(const std::string &stringID) {
	if (stringID.empty()) return getBlock(0);
	auto it = m_blocksID.find(stringID);
	if (it == m_blocksID.end())
		throw EXCEPTION("Unknown block:", stringID);
	return getBlock(it->second);
}

const Item &Registry::getItemFromStringID(const std::string &stringID) {
	if (stringID.empty()) return getItem(0);
	auto it = m_itemsID.find(stringID);
	if (it == m_itemsID.end())
		throw EXCEPTION("Unknown item:", stringID);
	return getItem(it->second);
}

const Sky &Registry::getSkyFromStringID(const std::string &stringID) {
	if (stringID.empty())
		throw EXCEPTION("Trying to get sky from empty string ID.");

	auto it = m_skiesID.find(stringID);
	if (it == m_skiesID.end())
		throw EXCEPTION("Unknown sky:", stringID);

	return getSky(it->second);
}

const Tree &Registry::getTreeFromStringID(const std::string &stringID) {
	if (stringID.empty())
		throw EXCEPTION("Trying to get tree from empty string ID.");

	auto it = m_treesID.find(stringID);
	if (it == m_treesID.end())
		throw EXCEPTION("Unknown tree:", stringID);

	return getTree(it->second);
}

const Biome &Registry::getBiomeFromStringID(const std::string &stringID) {
	if (stringID.empty())
		throw EXCEPTION("Trying to get biome from empty string ID.");

	auto it = m_biomesID.find(stringID);
	if (it == m_biomesID.end())
		throw EXCEPTION("Unknown biome:", stringID);

	return getBiome(it->second);
}

entt::entity Registry::getEntityFromStringID(const std::string &stringID) {
	if (stringID.empty()) {
		gkError() << "Failed to get entity from empty string ID";
		return entt::null;
	}

	auto it = m_entities.find(stringID);
	if (it == m_entities.end()) {
		gkError() << "Failed to get entity '" + stringID + "': Not found";
		return entt::null;
	}

	return it->second;
}

const Recipe *Registry::getRecipe(const Inventory &inventory) const {
	for (auto &recipe : m_recipes) {
		if (recipe->isMatching(inventory))
			return recipe.get();
	}
	return nullptr;
}

void Registry::serialize(sf::Packet &packet) const {
	for (auto &it : m_items) {
		packet << u8(DataType::Item) << it;
	}

	for (auto &it : m_blocks) {
		packet << u8(DataType::Block) << *it;
	}

	for (auto &it : m_recipes) {
		packet << u8((it->type() == "craft") ? DataType::CraftingRecipe : DataType::SmeltingRecipe)
			<< *it;
	}

	for (auto &it : m_skies) {
		packet << u8(DataType::Sky) << it;
	}

	for (auto &it : m_trees) {
		packet << u8(DataType::Tree) << it;
	}

	for (auto &it : m_biomes) {
		packet << u8(DataType::Biome) << it;
	}

	for (auto &it : m_dimensions) {
		packet << u8(DataType::Dimension) << it;
	}
}

void Registry::deserialize(sf::Packet &packet) {
	while (!packet.endOfPacket()) {
		u8 type;
		packet >> type;
		if (type == u8(DataType::Block)) {
			registerSerializedBlock<Block>(packet);
		}
		else if (type == u8(DataType::Item)) {
			registerSerializedItem(packet);
		}
		else if (type == u8(DataType::CraftingRecipe)) {
			registerRecipe<CraftingRecipe>()->deserialize(packet);
		}
		else if (type == u8(DataType::SmeltingRecipe)) {
			registerRecipe<SmeltingRecipe>()->deserialize(packet);
		}
		else if (type == u8(DataType::Sky)) {
			registerSerializedSky(packet);
		}
		else if (type == u8(DataType::Tree)) {
			registerSerializedTree(packet);
		}
		else if (type == u8(DataType::Biome)) {
			registerSerializedBiome(packet);
		}
		else if (type == u8(DataType::Dimension)) {
			registerSerializedDimension(packet);
		}
	}
}

