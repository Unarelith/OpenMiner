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
#ifndef BLOCKPLACEMENTCONSTRAINTS_HPP_
#define BLOCKPLACEMENTCONSTRAINTS_HPP_

#include <vector>

#include <gk/core/Vector3.hpp>

#include "ISerializable.hpp"
#include "NetworkUtils.hpp"

struct BlockPlacementConstraint : public ISerializable {
	gk::Vector3i blockOffset{0, 0, 0};
	std::string blockID;
	bool isWhitelist = true;

	void serialize(sf::Packet &packet) const override { packet << blockOffset << blockID << isWhitelist; }
	void deserialize(sf::Packet &packet) override { packet >> blockOffset >> blockID >> isWhitelist; }
};

class World;

class BlockPlacementConstraints : public ISerializable {
	public:
		bool check(const World &world, const gk::Vector3i &pos) const;

		void addConstraint(const BlockPlacementConstraint &constraint) { m_constraints.emplace_back(constraint); }

		void serialize(sf::Packet &packet) const override;
		void deserialize(sf::Packet &packet) override;

	private:
		std::vector<BlockPlacementConstraint> m_constraints;
};

#endif // BLOCKPLACEMENTCONSTRAINTS_HPP_
