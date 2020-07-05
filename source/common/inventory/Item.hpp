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
#ifndef ITEM_HPP_
#define ITEM_HPP_

#include <string>

#include <gk/core/IntTypes.hpp>
#include <gk/core/ISerializable.hpp>

#include "ItemType.hpp"
#include "TilesDef.hpp"

class Item : public gk::ISerializable {
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

		u8 harvestCapability() const { return m_harvestCapability; }
		void setHarvestCapability(u8 harvestCapability) { m_harvestCapability = harvestCapability; }

		float miningSpeed() const { return m_miningSpeed; }
		void setMiningSpeed(float miningSpeed) { m_miningSpeed = miningSpeed; }

		void addGroup(const std::string &name, u16 value) { m_groups.emplace(name, value); }
		bool hasGroup(const std::string &name) const { return m_groups.find(name) != m_groups.end(); }

		u16 getGroupValue(const std::string &name) const {
			auto it = m_groups.find(name);
			if (it == m_groups.end())
				return 0;

			return it->second;
		}

		bool canBeActivated() const { return m_canBeActivated; }

		static void initUsertype(sol::state &lua);

	protected:
		bool m_isBlock = false;

		bool m_canBeActivated = false;

	private:
		u32 m_id = 0;
		TilesDef m_tiles;

		std::string m_stringID;
		std::string m_label;

		u8 m_harvestCapability = 0;
		float m_miningSpeed = 1;

		std::unordered_map<std::string, u16> m_groups;
};

#endif // ITEM_HPP_
