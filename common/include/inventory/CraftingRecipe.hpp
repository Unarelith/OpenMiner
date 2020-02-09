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
#ifndef CRAFTINGRECIPE_HPP_
#define CRAFTINGRECIPE_HPP_

#include <array>
#include <map>

#include "Recipe.hpp"

class CraftingRecipe : public Recipe {
	public:
		CraftingRecipe() : Recipe("craft") {}
		CraftingRecipe(const std::vector<std::string> &pattern, const std::map<char, std::vector<std::string>> &keys, const ItemStack &result);

		void serialize(sf::Packet &packet) const override;
		void deserialize(sf::Packet &packet) override;

		bool isMatching(const Inventory &inventory) const override;

	private:
		bool checkMatch(const Inventory &inventory, int offsetX, int offsetY) const;

		std::vector<std::string> m_pattern;
		std::map<char, std::vector<std::string>> m_keys;
};

#endif // CRAFTINGRECIPE_HPP_
