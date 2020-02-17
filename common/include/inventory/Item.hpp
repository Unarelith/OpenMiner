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
#ifndef ITEM_HPP_
#define ITEM_HPP_

#include <string>

#include <gk/core/IntTypes.hpp>

#include "ItemType.hpp"
#include "ISerializable.hpp"
#include "TilesDef.hpp"

class Item : public ISerializable {
	public:
		Item() = default;
		Item(u32 id, const TilesDef &tiles, const std::string &stringID, const std::string &label);

		void serialize(sf::Packet &packet) const override;
		void deserialize(sf::Packet &packet) override;

		const std::string &stringID() const { return m_stringID; }
		const std::string &label() const { return m_label; }

		std::string modName() const { return m_stringID.substr(0, m_stringID.find_first_of(":")); }

		u32 id() const { return m_id; }
		const TilesDef &tiles() const { return m_tiles; }

		bool isBlock() const { return m_isBlock; }
		void setIsBlock(bool isBlock) { m_isBlock = isBlock; }

		bool isFuel() const { return m_isFuel; }
		void setIsFuel(bool isFuel) { m_isFuel = isFuel; }

		u16 burnTime() const { return m_burnTime; }
		void setBurnTime(u16 burnTime) { m_burnTime = burnTime; }

		u8 harvestCapability() const { return m_harvestCapability; }
		void setHarvestCapability(u8 harvestCapability) { m_harvestCapability = harvestCapability; }

		float miningSpeed() const { return m_miningSpeed; }
		void setMiningSpeed(float miningSpeed) { m_miningSpeed = miningSpeed; }

	protected:
		bool m_isBlock = false;
		bool m_isFuel = false;

		u16 m_burnTime = 0;

	private:
		u32 m_id = 0;
		TilesDef m_tiles;

		std::string m_stringID;
		std::string m_label;

		u8 m_harvestCapability = 0;
		float m_miningSpeed = 1;
};

#endif // ITEM_HPP_
