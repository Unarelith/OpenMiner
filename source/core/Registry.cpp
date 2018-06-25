/*
 * =====================================================================================
 *
 *       Filename:  Registry.cpp
 *
 *    Description:
 *
 *        Created:  23/06/2018 22:43:56
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "BlockWater.hpp"
#include "Registry.hpp"

Registry *Registry::s_instance = nullptr;

void Registry::registerBlocks() {
	registerBlock<Block>(0);
	registerBlock<Block>(1);
	registerBlock<Block>(2);
	registerBlock<Block>(3);
	registerBlock<Block>(4);
	registerBlock<Block>(5);
	registerBlock<Block>(6);
	registerBlock<Block>(7);
	registerBlock<BlockWater>(8);
	registerBlock<Block>(9);
	registerBlock<Block>(10);
	registerBlock<Block>(11);
	registerBlock<Block>(12);
	registerBlock<Block>(13);
	registerBlock<Block>(14);
	registerBlock<Block>(15);
	registerBlock<Block>(16);
	registerBlock<Block>(17);
	registerBlock<Block>(18);
	registerBlock<Block>(19);
	registerBlock<Block>(20);
	registerBlock<Block>(21);
	registerBlock<Block>(22);
	registerBlock<Block>(23);
	registerBlock<Block>(24);
}

void Registry::registerItems() {
	registerItem<Item>(0, 0, true);
	registerItem<Item>(1, 1, true);
	registerItem<Item>(2, 2, true);
	registerItem<Item>(3, 3, true);
	registerItem<Item>(4, 4, true);
	registerItem<Item>(5, 5, true);
	registerItem<Item>(6, 6, true);
	registerItem<Item>(7, 7, true);
	registerItem<Item>(8, 8, true);
	registerItem<Item>(9, 9, true);
	registerItem<Item>(10, 10, true);
	registerItem<Item>(11, 11, true);
	registerItem<Item>(12, 12, true);
	registerItem<Item>(13, 13, true);
	registerItem<Item>(14, 14, true);
	registerItem<Item>(15, 15, true);
	registerItem<Item>(16, 16, true);
	registerItem<Item>(17, 17, true);
	registerItem<Item>(18, 18, true);
	registerItem<Item>(19, 19, true);
	registerItem<Item>(20, 20, true);
	registerItem<Item>(21, 21, true);
	registerItem<Item>(22, 22, true);
	registerItem<Item>(23, 23, true);
	registerItem<Item>(24, 24, true);

	registerItem<Item>(25, 324);
	registerItem<Item>(26, 325);
	registerItem<Item>(27, 326);
	registerItem<Item>(28, 327);
	registerItem<Item>(29, 328);
	registerItem<Item>(30, 329);

	// FIXME: Move this to Application or load from XML file
	registerRecipes();
}

void Registry::registerRecipes() {
	m_recipes.emplace_back(std::array<u32, 9>{6, 6, 0, 6, 25, 0, 0, 25, 0}, ItemStack{26});
	m_recipes.emplace_back(std::array<u32, 9>{6, 6, 0, 0, 25, 0, 0, 25, 0}, ItemStack{27});
	m_recipes.emplace_back(std::array<u32, 9>{6, 6, 6, 0, 25, 0, 0, 25, 0}, ItemStack{28});
	m_recipes.emplace_back(std::array<u32, 9>{0, 6, 0, 0, 25, 0, 0, 25, 0}, ItemStack{29});
	m_recipes.emplace_back(std::array<u32, 9>{0, 6, 0, 0, 6, 0, 0, 25, 0}, ItemStack{30});

	m_recipes.emplace_back(std::array<u32, 9>{19, 19, 0, 0, 0, 0, 0, 0, 0}, ItemStack{25, 4}, true);
	m_recipes.emplace_back(std::array<u32, 9>{5, 0, 0, 0, 0, 0, 0, 0, 0}, ItemStack{19, 4}, true);
}

const CraftingRecipe *Registry::getRecipe(const Inventory &inventory) const {
	for (const CraftingRecipe &recipe : m_recipes) {
		if (recipe.isMatching(inventory))
			return &recipe;
	}
	return nullptr;
}

