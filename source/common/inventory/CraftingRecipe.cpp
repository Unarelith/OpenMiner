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
#include <algorithm>

#include "core/EngineConfig.hpp"
#include "core/Exception.hpp"
#include "inventory/CraftingRecipe.hpp"
#include "network/Network.hpp"

CraftingRecipe::CraftingRecipe(const std::vector<std::string> &pattern, const std::map<char, std::vector<std::string>> &keys, const ItemStack &result) : Recipe("craft", result) {
	m_pattern = pattern;
	m_keys = keys;
}

void CraftingRecipe::serialize(sf::Packet &packet) const {
	packet << m_result << u8(m_pattern.size());
	for (const std::string &it : m_pattern)
		packet << it;

	packet << u8(m_keys.size());
	for (auto &it : m_keys) {
		packet << u8(it.first) << u8(it.second.size());
		for (const std::string &str : it.second) {
			packet << str;
		}
	}
}

void CraftingRecipe::deserialize(sf::Packet &packet) {
	u8 patternSize, keysSize;

	packet >> m_result >> patternSize;
	for (u8 i = 0 ; i < patternSize ; ++i) {
		std::string str;
		packet >> str;
		m_pattern.emplace_back(str);
	}

	packet >> keysSize;
	for (u8 i = 0 ; i < keysSize ; ++i) {
		u8 key, itemsSize;
		packet >> key >> itemsSize;

		std::vector<std::string> items;
		for (u8 j = 0 ; j < itemsSize ; ++j) {
			std::string str;
			packet >> str;
			items.emplace_back(str);
		}

		m_keys.emplace(key, items);
	}
}

bool CraftingRecipe::isMatching(const Inventory &inventory) const {
	if (inventory.height() < m_pattern.size())
		return false;

	for (u16 y = 0 ; y < inventory.height() - m_pattern.size() + 1 ; ++y) {
		if (inventory.width() < m_pattern.at(0).size())
			return false;

		for (u16 x = 0 ; x < inventory.width() - m_pattern.at(0).size() + 1 ; ++x)
			if (checkMatch(inventory, x, y))
				return true;
	}

	return false;
}

bool CraftingRecipe::checkMatch(const Inventory &inventory, u16 offsetX, u16 offsetY) const {
	bool itemFound = false;
	u16 y = 0;
	for (y = 0 ; y < m_pattern.size() ; ++y) {
		u16 x = 0;
		for (x = 0 ; x < m_pattern[y].size() ; ++x) {
			itemFound = false;

			const Item &item = inventory.getStack(offsetX + x, offsetY + y).item();
			if (m_pattern[y][x] == ' ') {
				itemFound = (item.stringID().empty() || item.stringID() == BLOCK_AIR);
			}
			else {
				auto it = m_keys.find(m_pattern[y][x]);
				if (it == m_keys.end())
					throw EXCEPTION("Recipe error: Invalid key char(", (int)m_pattern[y][x], ")'");

				for (const std::string &itemID : it->second) {
					if (itemID == item.stringID() || (itemID.substr(0, 6) == "group:" && item.hasGroup(itemID)))
						itemFound = true;
				}
			}

			if (!itemFound)
				return false;
		}

		for (u16 x = 0 ; x < offsetX ; ++x) {
			if (inventory.getStack(x, offsetY + y).item().id())
				return false;
		}

		while (offsetX + x < inventory.width()) {
			if (inventory.getStack(offsetX + x, offsetY + y).item().id())
				return false;
			++x;
		}
	}

	for (u16 y = 0 ; y < offsetY ; ++y) {
		for (u16 x = 0 ; x < inventory.width() ; ++x) {
			if (inventory.getStack(x, y).item().id())
				return false;
		}
	}

	while (offsetY + y < inventory.height()) {
		for (u16 x = 0 ; x < inventory.width() ; ++x) {
			if (inventory.getStack(x, offsetY + y).item().id())
				return false;
		}
		++y;
	}

	return true;
}

