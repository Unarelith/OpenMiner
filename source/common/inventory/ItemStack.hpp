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
#ifndef ITEMSTACK_HPP_
#define ITEMSTACK_HPP_

#include "EngineConfig.hpp"
#include "Item.hpp"

namespace sf { class Packet; }

class ItemStack {
	public:
		ItemStack() = default;
		ItemStack(const std::string &stringID, u16 amount = 1)
			: m_stringID(stringID), m_amount(amount) {}

		const Item &item() const;
		void setItem(const std::string &stringID) { m_stringID = stringID; }

		u16 amount() const { return m_amount; }
		void setAmount(u16 amount) { m_amount = amount; }

		static void initUsertype(sol::state &lua);

		static const ItemStack Empty;

	private:
		std::string m_stringID{BLOCK_AIR};

		u16 m_amount = 0;
};

sf::Packet &operator<<(sf::Packet &packet, const ItemStack &itemStack);
sf::Packet &operator>>(sf::Packet &packet, ItemStack &itemStack);

#endif // ITEMSTACK_HPP_
