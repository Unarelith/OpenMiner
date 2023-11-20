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
#ifndef REGISTRY_HPP_
#define REGISTRY_HPP_

#include <memory>
#include <unordered_map>
#include <vector>

#include <entt/entt.hpp>

#include "common/inventory/Item.hpp"
#include "common/inventory/Recipe.hpp"
#include "common/network/Network.hpp"
#include "common/scene/EntityCallbackContainer.hpp"
#include "common/world/Biome.hpp"
#include "common/world/Block.hpp"
#include "common/world/Dimension.hpp"
#include "common/world/Key.hpp"
#include "common/world/Sky.hpp"
#include "common/world/Tree.hpp"

class Registry : public ISerializable {
	public:
		template<typename T>
		auto registerBlock(const std::string &stringID) -> typename std::enable_if<std::is_base_of<Block, T>::value, T&>::type {
			u16 id = (u16)m_blocks.size();
			m_blocksID.emplace(stringID, id);
			m_blocks.emplace_back(std::make_unique<T>(id, stringID));
			return *static_cast<T*>(m_blocks.back().get());
		}

		template<typename T>
		auto registerSerializedBlock(sf::Packet &packet) -> typename std::enable_if<std::is_base_of<Block, T>::value, T&>::type {
			m_blocks.emplace_back(std::make_unique<T>());
			m_blocks.back()->deserialize(packet);

			u16 id = u16(m_blocks.size() - 1);
			m_blocksID.emplace(m_blocks.back()->stringID(), id);

			return *static_cast<T*>(m_blocks.back().get());
		}

		template<typename T>
		auto registerItem(const TilesDef &tiles, const std::string &stringID, const std::string &label) -> typename std::enable_if<std::is_base_of<Item, T>::value, T&>::type {
			u16 id = (u16)m_items.size();
			m_itemsID.emplace(stringID, id);
			m_items.emplace_back(std::make_unique<T>(id, tiles, stringID, label));
			return *static_cast<T*>(m_items.back().get());
		}

		template<typename T>
		auto registerSerializedItem(sf::Packet &packet) -> typename std::enable_if<std::is_base_of<Item, T>::value, T&>::type {
			m_items.emplace_back(std::make_unique<T>());
			m_items.back()->deserialize(packet);

			u16 id = u16(m_items.size() - 1);
			m_itemsID.emplace(m_items.back()->stringID(), id);

			return *static_cast<T*>(m_items.back().get());
		}

		template<typename T, typename... Args>
		auto registerRecipe(Args &&...args) -> typename std::enable_if<std::is_base_of<Recipe, T>::value, Recipe*>::type {
			m_recipes.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
			return m_recipes.back().get();
		}

		Sky &registerSky(const std::string &stringID);
		Sky &registerSerializedSky(sf::Packet &packet);

		Tree &registerTree(const std::string &stringID);
		Tree &registerSerializedTree(sf::Packet &packet);

		Biome &registerBiome(const std::string &stringID, const std::string &label);
		Biome &registerSerializedBiome(sf::Packet &packet);

		Dimension &registerDimension(const std::string &stringID, const std::string &label);
		Dimension &registerSerializedDimension(sf::Packet &packet);

		Key &registerKey(const std::string &stringID, const std::string &label);
		Key &registerSerializedKey(sf::Packet &packet);

		entt::registry &entityRegistry() { return m_entityRegistry; }
		entt::entity registerEntity(const std::string &stringID);

		const Block &getBlock(u16 id) const { return *m_blocks.at(id).get(); }
		const Item &getItem(u16 id) const { return *m_items.at(id).get(); }
		const Sky &getSky(u16 id) const { return m_skies.at(id); }
		const Tree &getTree(u16 id) const { return m_trees.at(id); }
		const Biome &getBiome(u16 id) const { return m_biomes.at(id); }
		const Dimension &getDimension(u16 id) const { return m_dimensions.at(id); }
		const Key &getKey(u16 id) const;

		bool hasBlock(const std::string &stringID) const { return m_blocksID.find(stringID) != m_blocksID.end(); }
		bool hasItem(const std::string &stringID) const { return m_itemsID.find(stringID) != m_itemsID.end(); }

		const Block &getBlockFromStringID(const std::string &stringID);
		const Item &getItemFromStringID(const std::string &stringID);
		const Sky &getSkyFromStringID(const std::string &stringID);
		const Tree &getTreeFromStringID(const std::string &stringID);
		const Biome &getBiomeFromStringID(const std::string &stringID);
		entt::entity getEntityFromStringID(const std::string &stringID);

		const Recipe *getRecipe(const Inventory &inventory) const;

		EntityCallbackContainer &addEntityCallbackContainer(const std::string &stringID);
		EntityCallbackContainer &getEntityCallbackContainer(const std::string &stringID);

		void serialize(sf::Packet &packet) const override;
		void deserialize(sf::Packet &packet) override;

		void clear();

		static void initUsertype(sol::state &lua);

		const std::vector<std::shared_ptr<Block>> &blocks() const { return m_blocks; }
		const std::vector<std::shared_ptr<Item>> &items() const { return m_items; }
		const std::vector<Tree> &trees() const { return m_trees; }
		const std::vector<Biome> &biomes() const { return m_biomes; }
		const std::vector<Dimension> &dimensions() const { return m_dimensions; }
		std::vector<Key> &keys() { return m_keys; }

		static Registry &getInstance() { return *s_instance; }
		static void setInstance(Registry &instance) { s_instance = &instance; }

	private:
		static Registry *s_instance;

		std::vector<std::shared_ptr<Block>> m_blocks;
		std::vector<std::shared_ptr<Item>> m_items;
		std::vector<std::shared_ptr<Recipe>> m_recipes;
		std::vector<Sky> m_skies;
		std::vector<Tree> m_trees;
		std::vector<Biome> m_biomes;
		std::vector<Dimension> m_dimensions;
		std::vector<Key> m_keys;

		std::unordered_map<std::string, u16> m_blocksID;
		std::unordered_map<std::string, u16> m_itemsID;
		std::unordered_map<std::string, u16> m_skiesID;
		std::unordered_map<std::string, u16> m_treesID;
		std::unordered_map<std::string, u16> m_biomesID;
		std::unordered_map<std::string, u16> m_dimensionsID;
		std::unordered_map<std::string, u16> m_keysID;

		std::unordered_map<std::string, entt::entity> m_entities;
		std::unordered_map<std::string, EntityCallbackContainer> m_entityCallbacks;

		entt::registry m_entityRegistry;

		enum class DataType {
			Block,
			Item,
			CraftingRecipe,
			SmeltingRecipe,
			Sky,
			Tree,
			Biome,
			Dimension,
			Key
		};
};

#endif // REGISTRY_HPP_
