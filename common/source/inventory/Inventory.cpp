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
#include "Inventory.hpp"
#include "Network.hpp"

void Inventory::setStack(u16 x, u16 y, const std::string &name, u16 amount) {
	m_items.at(x + y * m_width) = ItemStack(name, amount);
	m_hasChanged = true;
}

void Inventory::addStack(const std::string &name, u16 amount) {
	for (std::size_t i = 0 ; i < m_items.size() ; ++i) {
		if (m_items[i].item().id() == 0) {
			m_items[i] = ItemStack(name, amount);
			m_hasChanged = true;
			break;
		}
		else if (m_items[i].item().name() == name) {
			m_items[i] = ItemStack(name, m_items[i].amount() + amount);
			m_hasChanged = true;
			break;
		}
	}
}

void Inventory::serialize(sf::Packet &packet) const {
	packet << m_width << m_height << u8(m_inBlock)
		<< s32(m_blockPos.x) << s32(m_blockPos.y) << s32(m_blockPos.z);

	int i = 0;
	for (auto &it : m_items) {
		packet << it.item().name() << it.amount()
			<< u8(i % m_width) << u8(i / m_width);
		++i;
	}
}

void Inventory::deserialize(sf::Packet &packet) {
	u8 inBlock;
	s32 bx, by, bz;
	packet >> m_width >> m_height >> inBlock >> bx >> by >> bz;
	m_inBlock = inBlock;
	m_blockPos = gk::Vector3i{bx, by, bz};

	if (m_items.size() != m_width * m_height)
		m_items.resize(m_width * m_height);

	std::string name;
	u16 amount;
	u8 x, y;
	while (!packet.endOfPacket()) {
		packet >> name >> amount >> x >> y;
		setStack(x, y, name, amount);
	}
}

