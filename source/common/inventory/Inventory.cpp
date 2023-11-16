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
#include "core/Debug.hpp"
#include "core/EngineConfig.hpp"
#include "core/Registry.hpp"
#include "inventory/Inventory.hpp"
#include "network/Network.hpp"

void Inventory::setStack(u16 x, u16 y, const std::string &stringID, u16 amount) {
	m_items.at(x + y * m_width) = ItemStack(stringID, amount);
	m_hasChanged = true;
}

ItemStack Inventory::addStack(const std::string &stringID, u16 amount, u16 offset, u16 size, bool mergeOnly, bool retryAfterMerge) {
	if (stringID.empty() || stringID == BLOCK_AIR || amount == 0)
		return ItemStack::Empty;

	bool hasChanged = false;

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

			hasChanged = true;
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

			hasChanged = true;
		}
	}

	if (!hasChanged && mergeOnly && retryAfterMerge && ret.amount())
		return addStack(ret.item().stringID(), ret.amount(), offset, size);
	else if (hasChanged)
		m_hasChanged = hasChanged;

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
	m_blockPos = Vector3i{bx, by, bz};

	if (m_items.size() != m_width * m_height)
		m_items.resize(m_width * m_height);

	u16 itemListSize, i = 0;
	packet >> itemListSize;

	std::string name;
	u16 amount;
	u8 x, y;
	while (i < itemListSize) {
		packet >> name >> amount >> x >> y;

		if (Registry::getInstance().hasItem(name))
			setStack(x, y, name, amount);
		else
			logError() << "Inventory::deserialize: Failed to find item in registry:" << name;

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

