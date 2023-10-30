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
#ifndef TREE_HPP_
#define TREE_HPP_

#include <string>

#include <gk/core/IntTypes.hpp>

#include "ISerializable.hpp"

class Tree : public ISerializable {
	public:
		Tree() = default;
		Tree(u16 id, const std::string &stringID);

		void serialize(sf::Packet &packet) const override;
		void deserialize(sf::Packet &packet) override;

		u16 id() const { return m_id; }
		const std::string &stringID() const { return m_stringID; }

		u16 getLogBlockID() const { return m_logBlockID; }
		u16 getLeavesBlockID() const { return m_leavesBlockID; }

		u8 trunkMinHeight() const { return m_trunkMinHeight; }
		u8 trunkMaxHeight() const { return m_trunkMaxHeight; }

		bool hasLeaves() const { return m_hasLeaves; }

		void setLogBlockID(u16 value) { m_logBlockID = value; }
		void setLeavesBlockID(u16 value) { m_leavesBlockID = value; }

		void setTrunkHeight(u8 min, u8 max) { m_trunkMinHeight = min; m_trunkMaxHeight = max; }

		void setHasLeaves(bool hasLeaves) { m_hasLeaves = hasLeaves; }

	private:
		u16 m_id = 0;
		std::string m_stringID;

		u16 m_logBlockID = 0;
		u16 m_leavesBlockID = 0;

		u8 m_trunkMinHeight = 0;
		u8 m_trunkMaxHeight = 0;

		bool m_hasLeaves = true;
};

#endif // TREE_HPP_
