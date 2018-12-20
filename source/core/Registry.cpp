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
#include "CraftingRecipe.hpp"
#include "ItemBlock.hpp"
#include "Registry.hpp"
#include "SmeltingRecipe.hpp"

#include "BlockFurnace.hpp"
#include "BlockWater.hpp"
#include "BlockWorkbench.hpp"
#include "XMLFile.hpp"

Registry *Registry::s_instance = nullptr;

const Recipe *Registry::getRecipe(const Inventory &inventory) const {
	for (auto &recipe : m_recipes) {
		if (recipe->isMatching(inventory))
			return recipe.get();
	}
	return nullptr;
}

