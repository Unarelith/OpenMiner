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
#include <unordered_map>
#include <vector>

#include "Block.hpp"
#include "Item.hpp"
#include "Network.hpp"
#include "Recipe.hpp"

struct Client;

class Registry {
	public:
		Block &registerBlock(u32 textureID, const std::string &id, const std::string &name);
		Item &registerItem(u32 textureID, const std::string &id, const std::string &name);

		template<typename T, typename... Args>
		auto registerRecipe(Args &&...args) -> typename std::enable_if<std::is_base_of<Recipe, T>::value, Recipe*>::type {
			return m_recipes.emplace_back(std::make_unique<T>(std::forward<Args>(args)...)).get();
		}

		const Block &getBlock(std::size_t id) const { return m_blocks.at(id); }
		const Item &getItem(std::size_t id) const { return m_items.at(id); }

		const Block &getBlock(const std::string &id);
		const Item &getItem(const std::string &id);

		const Recipe *getRecipe(const Inventory &inventory) const;

		void serialize(sf::Packet &packet);
		void deserialize(sf::Packet &packet);

		static Registry &getInstance() { return *s_instance; }
		static void setInstance(Registry &instance) { s_instance = &instance; }

	private:
		static Registry *s_instance;

		std::vector<Block> m_blocks;
		std::vector<Item> m_items;
		std::vector<std::unique_ptr<Recipe>> m_recipes;

		std::unordered_map<std::string, u32> m_blocksID;
		std::unordered_map<std::string, u32> m_itemsID;

		enum class DataType {
			Block,
			Item,
			Recipe
		};
};

#endif // REGISTRY_HPP_
