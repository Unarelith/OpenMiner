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
#ifndef PLACEMENTENTRY_HPP_
#define PLACEMENTENTRY_HPP_

#include <SFML/Network/Packet.hpp>

#include <gk/core/IntTypes.hpp>
#include <gk/core/ISerializable.hpp>

namespace PlacementEntry {

struct Flora : public gk::ISerializable {
	u16 blockID;
	u16 spawnsOnBlockID;
	double probability;

	void serialize(sf::Packet &packet) const override {
		packet << blockID << spawnsOnBlockID << probability;
	}

	void deserialize(sf::Packet &packet) override {
		packet >> blockID >> spawnsOnBlockID >> probability;
	}
};

struct Ore : public gk::ISerializable {
	enum Gen {
		RandomWalk,
		FloodFill
	};

	u16 blockID;
	double probability;
	double size;
	u8 genType;

	void serialize(sf::Packet &packet) const override {
		packet << blockID << probability << size << genType;
	}

	void deserialize(sf::Packet &packet) override {
		packet >> blockID >> probability >> size >> genType;
	}
};

struct Tree : public gk::ISerializable {
	u16 treeID;
	double probability;

	void serialize(sf::Packet &packet) const override {
		packet << treeID << probability;
	}

	void deserialize(sf::Packet &packet) override {
		packet >> treeID >> probability;
	}
};

} // namespace PlacementEntry

#endif // PLACEMENTENTRY_HPP_
