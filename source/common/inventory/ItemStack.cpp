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
#include "common/core/Registry.hpp"
#include "common/inventory/ItemStack.hpp"

const ItemStack ItemStack::Empty{BLOCK_AIR, 0};

const Item &ItemStack::item() const {
	return Registry::getInstance().getItemFromStringID(m_stringID);
}

// Please update 'docs/lua-api-cpp.md' if you change this
void ItemStack::initUsertype(sol::state &lua) {
	lua.new_usertype<ItemStack>("ItemStack",
		sol::constructors<ItemStack(const std::string &, u16)>(),
		"amount", &ItemStack::amount,
		"item", &ItemStack::item
	);
}

sf::Packet &operator<<(sf::Packet &packet, const ItemStack &itemStack) {
	return packet << itemStack.item().stringID() << itemStack.amount();
}

sf::Packet &operator>>(sf::Packet &packet, ItemStack &itemStack) {
	std::string stringID;
	u16 amount;
	packet >> stringID >> amount;
	itemStack.setItem(stringID);
	itemStack.setAmount(amount);
	return packet;
}

