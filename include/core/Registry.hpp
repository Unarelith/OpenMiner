/*
 * =====================================================================================
 *
 *       Filename:  Registry.hpp
 *
 *    Description:
 *
 *        Created:  23/06/2018 22:32:51
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef REGISTRY_HPP_
#define REGISTRY_HPP_

#include <memory>
#include <vector>

#include <sol.hpp>

#include "Block.hpp"
#include "Exception.hpp"
#include "Item.hpp"
#include "Recipe.hpp"

class Registry {
	public:
		void registerBlockFromTable(const sol::table &table);
		void registerItemFromTable(const sol::table &table);
		void registerCraftingRecipeFromTable(const sol::table &table);
		void registerSmeltingRecipeFromTable(const sol::table &table);

		const Block &getBlock(std::size_t id) const { return m_blocks.at(id); }
		const Item &getItem(std::size_t id) const { return m_items.at(id); }

		const Block &getBlock(const std::string &name) {
			if (name.empty()) return getBlock((int)0);
			auto it = m_blocksID.find(name);
			if (it == m_blocksID.end())
				throw EXCEPTION("Unknown block:", name);
			return getBlock(it->second);
		}

		const Item &getItem(const std::string &name) {
			if (name.empty()) return getItem((int)0);
			auto it = m_itemsID.find(name);
			if (it == m_itemsID.end())
				throw EXCEPTION("Unknown item:", name);
			return getItem(it->second);
		}

		const Recipe *getRecipe(const Inventory &inventory) const;

		static Registry &getInstance() { return *s_instance; }
		static void setInstance(Registry &instance) { s_instance = &instance; }

	private:
		Block &registerBlock(u32 textureID, const std::string &name, const std::string &label) {
			u32 id = m_blocks.size();
			m_blocksID.emplace(name, id);
			return m_blocks.emplace_back(id, textureID, name, label);
		}

		Item &registerItem(u32 textureID, const std::string &name, const std::string &label) {
			u32 id = m_items.size();
			m_itemsID.emplace(name, id);
			return m_items.emplace_back(id, textureID, name, label);
		}

		template<typename T, typename... Args>
		auto registerRecipe(Args &&...args) -> typename std::enable_if<std::is_base_of<Recipe, T>::value, Recipe*>::type {
			return m_recipes.emplace_back(std::make_unique<T>(std::forward<Args>(args)...)).get();
		}

		static Registry *s_instance;

		std::vector<Block> m_blocks;
		std::vector<Item> m_items;
		std::vector<std::unique_ptr<Recipe>> m_recipes;

		std::unordered_map<std::string, u32> m_blocksID;
		std::unordered_map<std::string, u32> m_itemsID;
};

#endif // REGISTRY_HPP_
