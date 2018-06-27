/*
 * =====================================================================================
 *
 *       Filename:  CraftingRecipe.hpp
 *
 *    Description:
 *
 *        Created:  24/06/2018 06:38:11
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef CRAFTINGRECIPE_HPP_
#define CRAFTINGRECIPE_HPP_

#include <array>
#include <map>

#include "Inventory.hpp"

class CraftingRecipe {
	public:
		CraftingRecipe(const std::vector<std::string> &pattern, const std::map<char, std::vector<u32>> &keys, const ItemStack &result, bool isShapeless = false);

		bool isMatching(const Inventory &inventory) const;

		const ItemStack &result() const { return m_result; }

	private:
		bool checkMatch(const Inventory &inventory, int offsetX, int offsetY) const;

		std::vector<std::string> m_pattern;
		std::map<char, std::vector<u32>> m_keys;
		ItemStack m_result;

		bool m_isShapeless;
};

#endif // CRAFTINGRECIPE_HPP_
