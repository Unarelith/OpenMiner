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
#include "EngineConfig.hpp"
#include "Inventory.hpp"
#include "Network.hpp"

void Inventory::setStack(u16 x, u16 y, const std::string &stringID, u16 amount) {
	m_items.at(x + y * m_width) = ItemStack(stringID, amount);
	m_hasChanged = true;
}

ItemStack Inventory::addStack(const std::string &stringID, u16 amount, u16 offset, u16 size, bool mergeOnly) {
	if (stringID.empty() || stringID == BLOCK_AIR || amount == 0)
		return {BLOCK_AIR, 0};

	ItemStack ret{stringID, amount};
	for (std::size_t i = offset ; ret.amount() && i < (size ? offset + size : m_items.size()) ; ++i) {
		const Item &item = m_items[i].item();
		if (item.id() == 0 && !mergeOnly) {
			if (ret.amount() > item.maxStackSize()) {
				m_items[i] = ItemStack(stringID, item.maxStackSize());
				ret.setAmount(ret.amount() - item.maxStackSize());
			}
			else {
				m_items[i] = ItemStack(stringID, ret.amount());
				ret.setAmount(0);
			}
			m_hasChanged = true;
		}
		else if (item.stringID() == stringID) {
			u16 sum = m_items[i].amount() + ret.amount();
			if (sum > item.maxStackSize()) {
				m_items[i] = ItemStack(stringID, item.maxStackSize());
				ret.setAmount(sum - item.maxStackSize());
			}
			else {
				m_items[i] = ItemStack(stringID, sum);
				ret.setAmount(0);
			}

			m_hasChanged = true;
		}
	}

	if (!m_hasChanged && mergeOnly && ret.amount())
		return addStack(ret.item().stringID(), ret.amount(), offset, size);

	return ret;
}

// NOTE: This fonction is only used by Lua since default parameters don't work properly
ItemStack Inventory::addStack2(const std::string &stringID, u16 amount) {
	return addStack(stringID, amount, 0, 0);
}

void Inventory::clearStack(u16 x, u16 y) {
	setStack(x, y, BLOCK_AIR, 0);
}

void Inventory::serialize(sf::Packet &packet) const {
	packet << m_width << m_height << m_name << u8(m_inBlock)
		<< s32(m_blockPos.x) << s32(m_blockPos.y) << s32(m_blockPos.z)
		<< m_isUnlimited;

	packet << u16(m_items.size());

	int i = 0;
	for (auto &it : m_items) {
		packet << it.item().stringID() << it.amount()
			<< u8(i % m_width) << u8(i / m_width);
		++i;
	}
}

void Inventory::deserialize(sf::Packet &packet) {
	u8 inBlock;
	s32 bx, by, bz;
	packet >> m_width >> m_height >> m_name >> inBlock >> bx >> by >> bz >> m_isUnlimited;

	m_inBlock = inBlock;
	m_blockPos = gk::Vector3i{bx, by, bz};

	if (m_items.size() != m_width * m_height)
		m_items.resize(m_width * m_height);

	u16 itemListSize, i = 0;
	packet >> itemListSize;

	std::string name;
	u16 amount;
	u8 x, y;
	while (i < itemListSize) {
		packet >> name >> amount >> x >> y;
		setStack(x, y, name, amount);
		++i;
	}
}

// Please update 'docs/lua-api-cpp.md' if you change this
void Inventory::initUsertype(sol::state &lua) {
	lua.new_usertype<Inventory>("Inventory",
		"add_stack", sol::overload(&Inventory::addStack, &Inventory::addStack2),
		"get_stack", &Inventory::getStack,
		"set_stack", &Inventory::setStack
	);
}

