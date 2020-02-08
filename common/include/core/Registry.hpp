/*
 * =====================================================================================
 *
 *  OpenMiner
 *  Copyright (C) 2018-2020 Unarelith, Quentin Bazin <openminer@unarelith.net>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *
 * =====================================================================================
 */
#ifndef REGISTRY_HPP_
#define REGISTRY_HPP_

#include <memory>
#include <unordered_map>
#include <vector>

#include "Block.hpp"
#include "Item.hpp"
#include "Network.hpp"
#include "Recipe.hpp"

class Registry : public ISerializable {
	public:
		template<typename T>
		auto registerBlock(const TilesDef &tiles, const std::string &id, const std::string &name) -> typename std::enable_if<std::is_base_of<Block, T>::value, T&>::type {
			u32 internalID = m_blocks.size();
			m_blocksID.emplace(id, internalID);
			m_blocks.emplace_back(std::make_unique<T>(internalID, tiles, id, name));
			return *static_cast<T*>(m_blocks.back().get());
		}

		template<typename T>
		auto registerSerializedBlock(sf::Packet &packet) -> typename std::enable_if<std::is_base_of<Block, T>::value, T&>::type {
			m_blocks.emplace_back(std::make_unique<T>());
			m_blocks.back()->deserialize(packet);

			u32 internalID = m_blocks.size() - 1;
			m_blocksID.emplace(m_blocks.back()->name(), internalID);

			return *static_cast<T*>(m_blocks.back().get());
		}

		Item &registerItem(const TilesDef &tiles, const std::string &id, const std::string &name);
		Item &registerSerializedItem(sf::Packet &packet);

		template<typename T, typename... Args>
		auto registerRecipe(Args &&...args) -> typename std::enable_if<std::is_base_of<Recipe, T>::value, Recipe*>::type {
			m_recipes.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
			return m_recipes.back().get();
		}

		const Block &getBlock(std::size_t id) const { return *m_blocks.at(id).get(); }
		const Item &getItem(std::size_t id) const { return m_items.at(id); }

		const Block &getBlockFromStringID(const std::string &id);
		const Item &getItemFromStringID(const std::string &id);

		const Recipe *getRecipe(const Inventory &inventory) const;

		void serialize(sf::Packet &packet) const override;
		void deserialize(sf::Packet &packet) override;

		const std::vector<std::unique_ptr<Block>> &blocks() const { return m_blocks; }
		const std::vector<Item> &items() const { return m_items; }

		static Registry &getInstance() { return *s_instance; }
		static void setInstance(Registry &instance) { s_instance = &instance; }

	private:
		static Registry *s_instance;

		std::vector<std::unique_ptr<Block>> m_blocks;
		std::vector<Item> m_items;
		std::vector<std::unique_ptr<Recipe>> m_recipes;

		std::unordered_map<std::string, u32> m_blocksID;
		std::unordered_map<std::string, u32> m_itemsID;

		enum class DataType {
			Block,
			Item,
			CraftingRecipe,
			SmeltingRecipe,
		};
};

#endif // REGISTRY_HPP_
