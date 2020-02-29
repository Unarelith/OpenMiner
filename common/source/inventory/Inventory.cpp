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
#include "Inventory.hpp"
#include "Network.hpp"

void Inventory::setStack(u16 x, u16 y, const std::string &stringID, u16 amount) {
	m_items.at(x + y * m_width) = ItemStack(stringID, amount);
	m_hasChanged = true;
}

bool Inventory::addStack(const std::string &stringID, u16 amount) {
	for (std::size_t i = 0 ; i < m_items.size() ; ++i) {
		if (m_items[i].item().id() == 0) {
			m_items[i] = ItemStack(stringID, amount);
			m_hasChanged = true;
			return true;
		}
		else if (m_items[i].item().stringID() == stringID) {
			m_items[i] = ItemStack(stringID, m_items[i].amount() + amount);
			m_hasChanged = true;
			return true;
		}
	}

	return false;
}

void Inventory::clearStack(u16 x, u16 y) {
	setStack(x, y, "_:air", 0);
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

