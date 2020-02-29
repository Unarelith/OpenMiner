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
#ifndef INVENTORY_HPP_
#define INVENTORY_HPP_

#include <vector>

#include <gk/core/Vector3.hpp>

#include "ItemStack.hpp"
#include "ISerializable.hpp"

class Inventory : public ISerializable {
	public:
		Inventory() = default;
		Inventory(u16 width, u16 height, const std::string &name = "")
			: m_name(name), m_width(width), m_height(height) { m_items.resize(width * height); }

		const ItemStack &getStack(u16 x, u16 y) const { return m_items.at(x + y * m_width); }
		ItemStack &getStackRef(u16 x, u16 y) { return m_items.at(x + y * m_width); }
		void setStack(u16 x, u16 y, const std::string &stringID, u16 amount = 1);
		void addStack(const std::string &stringID, u16 amount = 1);

		void serialize(sf::Packet &packet) const override;
		void deserialize(sf::Packet &packet) override;

		const std::string &name() const { return m_name; }

		u16 width() const { return m_width; }
		u16 height() const { return m_height; }
		void resize(u16 width, u16 height) { m_width = width; m_height = height; m_items.resize(width * height); }

		const std::vector<ItemStack> &items() const { return m_items; }

		bool inBlock() const { return m_inBlock; }
		void setInBlock(bool inBlock) { m_inBlock = inBlock; }

		const gk::Vector3i &blockPos() const { return m_blockPos; }
		void setBlockPos(const gk::Vector3i &blockPos) { m_blockPos = blockPos; }

		bool hasChanged() const { return m_hasChanged; }
		void setChanged(bool hasChanged) { m_hasChanged = hasChanged; }

		bool isUnlimited() const { return m_isUnlimited; }
		void setUnlimited(bool isUnlimited) { m_isUnlimited = isUnlimited; }

	private:
		std::string m_name;

		u16 m_width = 0;
		u16 m_height = 0;

		std::vector<ItemStack> m_items;

		bool m_inBlock = false;
		gk::Vector3i m_blockPos{0, 0, 0};

		bool m_hasChanged = false; // Used to send inventory update packets
		bool m_isUnlimited = false;
};

#endif // INVENTORY_HPP_
