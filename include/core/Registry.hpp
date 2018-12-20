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

#include "Block.hpp"
#include "Item.hpp"
#include "Recipe.hpp"

class Registry {
	public:
		template<typename T, typename... Args>
		auto registerBlock(Args &&...args) -> typename std::enable_if<std::is_base_of<Block, T>::value, Block*>::type {
			return m_blocks.emplace_back(std::make_unique<T>(std::forward<Args>(args)...)).get();
		}

		template<typename T, typename... Args>
		auto registerItem(Args &&...args) -> typename std::enable_if<std::is_base_of<Item, T>::value, Item*>::type {
			return m_items.emplace_back(std::make_unique<T>(std::forward<Args>(args)...)).get();
		}

		template<typename T, typename... Args>
		auto registerRecipe(Args &&...args) -> typename std::enable_if<std::is_base_of<Recipe, T>::value, Recipe*>::type {
			return m_recipes.emplace_back(std::make_unique<T>(std::forward<Args>(args)...)).get();
		}

		void registerRecipes();

		const Block &getBlock(std::size_t id) const { return *m_blocks.at(id).get(); }
		const Item &getItem(std::size_t id) const { return *m_items.at(id).get(); }

		const Recipe *getRecipe(const Inventory &inventory) const;

		static Registry &getInstance() { return *s_instance; }
		static void setInstance(Registry &instance) { s_instance = &instance; }

	private:
		static Registry *s_instance;

		std::vector<std::unique_ptr<Block>> m_blocks;
		std::vector<std::unique_ptr<Item>> m_items;
		std::vector<std::unique_ptr<Recipe>> m_recipes;
};

#endif // REGISTRY_HPP_
