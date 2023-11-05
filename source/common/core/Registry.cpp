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
#include "CraftingRecipe.hpp"
#include "Exception.hpp"
#include "GameKey.hpp"
#include "SmeltingRecipe.hpp"
#include "Registry.hpp"

Registry *Registry::s_instance = nullptr;

Sky &Registry::registerSky(const std::string &stringID) {
	u16 id = (u16)m_skies.size();
	m_skiesID.emplace(stringID, id);
	m_skies.emplace_back(id, stringID);
	return m_skies.back();
}

Sky &Registry::registerSerializedSky(sf::Packet &packet) {
	m_skies.emplace_back();
	m_skies.back().deserialize(packet);

	u16 id = u16(m_skies.size() - 1);
	m_skiesID.emplace(m_skies.back().stringID(), id);

	return m_skies.back();
}

Tree &Registry::registerTree(const std::string &stringID) {
	u16 id = (u16)m_trees.size();
	m_treesID.emplace(stringID, id);
	m_trees.emplace_back(id, stringID);
	return m_trees.back();
}

Tree &Registry::registerSerializedTree(sf::Packet &packet) {
	m_trees.emplace_back();
	m_trees.back().deserialize(packet);

	u16 id = u16(m_trees.size() - 1);
	m_treesID.emplace(m_trees.back().stringID(), id);

	return m_trees.back();
}

Biome &Registry::registerBiome(const std::string &stringID, const std::string &label) {
	u16 id = (u16)m_biomes.size();
	m_biomesID.emplace(stringID, id);
	m_biomes.emplace_back(id, stringID, label);
	return m_biomes.back();
}

Biome &Registry::registerSerializedBiome(sf::Packet &packet) {
	m_biomes.emplace_back();
	m_biomes.back().deserialize(packet);

	u16 id = u16(m_biomes.size() - 1);
	m_biomesID.emplace(m_biomes.back().stringID(), id);

	return m_biomes.back();
}

Dimension &Registry::registerDimension(const std::string &stringID, const std::string &label) {
	u16 id = (u16)m_dimensions.size();
	m_dimensionsID.emplace(stringID, id);
	m_dimensions.emplace_back(id, stringID, label);
	return m_dimensions.back();
}

Dimension &Registry::registerSerializedDimension(sf::Packet &packet) {
	m_dimensions.emplace_back();
	m_dimensions.back().deserialize(packet);

	u16 id = u16(m_dimensions.size() - 1);
	m_dimensionsID.emplace(m_dimensions.back().stringID(), id);

	return m_dimensions.back();
}

Key &Registry::registerKey(const std::string &stringID, const std::string &label) {
	u16 id = u16(GameKey::KeyCount + m_keys.size());
	m_keysID.emplace(stringID, id);
	m_keys.emplace_back(id, stringID, label);
	return m_keys.back();
}

Key &Registry::registerSerializedKey(sf::Packet &packet) {
	m_keys.emplace_back();
	m_keys.back().deserialize(packet);

	u16 id = u16(GameKey::KeyCount + m_keys.size() - 1);
	m_keysID.emplace(m_biomes.back().stringID(), id);

	return m_keys.back();
}

entt::entity Registry::registerEntity(const std::string &stringID) {
	auto it = m_entities.find(stringID);
	if (it == m_entities.end()) {
		entt::entity entity = m_entityRegistry.create();
		m_entities.emplace(stringID, entity);
		return entity;
	}
	else
		logError() << "Registry Error: Redefinition of entity '" + stringID + "', keeping the first one";

	return entt::null;
}

const Key &Registry::getKey(u16 id) const {
	return m_keys.at(id - GameKey::KeyCount);
}

const Block &Registry::getBlockFromStringID(const std::string &stringID) {
	if (stringID.empty()) return getBlock(0);
	auto it = m_blocksID.find(stringID);
	if (it == m_blocksID.end())
		throw EXCEPTION("Registry Error: Unknown block:", stringID);
	return getBlock(it->second);
}

const Item &Registry::getItemFromStringID(const std::string &stringID) {
	if (stringID.empty()) return getItem(0);
	auto it = m_itemsID.find(stringID);
	if (it == m_itemsID.end())
		throw EXCEPTION("Registry Error: Unknown item:", stringID);
	return getItem(it->second);
}

const Sky &Registry::getSkyFromStringID(const std::string &stringID) {
	if (stringID.empty())
		throw EXCEPTION("Registry Error: Trying to get sky from empty string ID.");

	auto it = m_skiesID.find(stringID);
	if (it == m_skiesID.end())
		throw EXCEPTION("Registry Error: Unknown sky:", stringID);

	return getSky(it->second);
}

const Tree &Registry::getTreeFromStringID(const std::string &stringID) {
	if (stringID.empty())
		throw EXCEPTION("Registry Error: Trying to get tree from empty string ID.");

	auto it = m_treesID.find(stringID);
	if (it == m_treesID.end())
		throw EXCEPTION("Registry Error: Unknown tree:", stringID);

	return getTree(it->second);
}

const Biome &Registry::getBiomeFromStringID(const std::string &stringID) {
	if (stringID.empty())
		throw EXCEPTION("Registry Error: Trying to get biome from empty string ID.");

	auto it = m_biomesID.find(stringID);
	if (it == m_biomesID.end())
		throw EXCEPTION("Registry Error: Unknown biome:", stringID);

	return getBiome(it->second);
}

entt::entity Registry::getEntityFromStringID(const std::string &stringID) {
	if (stringID.empty()) {
		logError() << "Registry Error: Failed to get entity from empty string ID";
		return entt::null;
	}

	auto it = m_entities.find(stringID);
	if (it == m_entities.end()) {
		logError() << ("Registry Error: Failed to get entity '" + stringID + "': Not found").c_str();
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

EntityCallbackContainer &Registry::addEntityCallbackContainer(const std::string &stringID) {
	return m_entityCallbacks.emplace(stringID, EntityCallbackContainer{}).first->second;
}

EntityCallbackContainer &Registry::getEntityCallbackContainer(const std::string &stringID) {
	return m_entityCallbacks.at(stringID);
}

void Registry::serialize(sf::Packet &packet) const {
	for (auto &it : m_blocks) {
		packet << u8(DataType::Block) << *it;
	}

	for (auto &it : m_items) {
		packet << u8(DataType::Item) << *it;
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

	for (auto &it : m_keys) {
		packet << u8(DataType::Key) << it;
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
			registerSerializedItem<Item>(packet);
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
		else if (type == u8(DataType::Key)) {
			registerSerializedKey(packet);
		}
	}
}

void Registry::clear() {
	m_blocks.clear();
	m_items.clear();
	m_recipes.clear();
	m_skies.clear();
	m_trees.clear();
	m_biomes.clear();
	m_dimensions.clear();
	m_keys.clear();

	m_blocksID.clear();
	m_itemsID.clear();
	m_skiesID.clear();
	m_treesID.clear();
	m_biomesID.clear();
	m_dimensionsID.clear();
	m_keysID.clear();

	m_entities.clear();
	m_entityCallbacks.clear();
	m_entityRegistry.clear();
}

// Please update 'docs/lua-api-cpp.md' if you change this
void Registry::initUsertype(sol::state &lua) {
	lua.new_usertype<Registry>("Registry",
		"get_block",  &Registry::getBlock,
		"get_item",   &Registry::getItem,
		"get_sky",    &Registry::getSky,
		"get_tree",   &Registry::getTree,
		"get_biome",  &Registry::getBiome,
		"get_recipe", &Registry::getRecipe,
		"get_key",    &Registry::getKey,

		"get_block_from_string", &Registry::getBlockFromStringID,
		"get_item_from_string",  &Registry::getItemFromStringID,
		"get_sky_from_string",   &Registry::getSkyFromStringID,
		"get_tree_from_string",  &Registry::getTreeFromStringID,
		"get_biome_from_string", &Registry::getBiomeFromStringID,

		"blocks", &Registry::blocks,
		"items", &Registry::items,
		"trees", &Registry::trees,
		"biomes", &Registry::biomes,
		"dimensions", &Registry::dimensions
	);
}

