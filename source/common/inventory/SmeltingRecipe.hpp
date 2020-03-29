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
#ifndef SMELTINGRECIPE_HPP_
#define SMELTINGRECIPE_HPP_

#include "Recipe.hpp"

class SmeltingRecipe : public Recipe {
	public:
		SmeltingRecipe() : Recipe("smelt") {}
		SmeltingRecipe(const std::string &inputID, u16 inputAmount, const ItemStack &output);

		void serialize(sf::Packet &packet) const override;
		void deserialize(sf::Packet &packet) override;

		bool isMatching(const Inventory &inventory) const override;

	private:
		std::string m_inputID;
		u16 m_inputAmount = 0;
};

#endif // SMELTINGRECIPE_HPP_
