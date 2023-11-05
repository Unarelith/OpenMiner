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
#ifndef POSITIONCOMPONENT_HPP_
#define POSITIONCOMPONENT_HPP_

#include "IntTypes.hpp"
#include "ISerializable.hpp"
#include "Network.hpp"

struct PositionComponent : public ISerializable {
	PositionComponent() = default;
	PositionComponent(double x_, double y_, double z_, u16 dimension_)
		: x(x_), y(y_), z(z_), dimension(dimension_) {}

	double x = 0;
	double y = 0;
	double z = 0;

	u16 dimension = 0;

	bool isUpdated = true;

	void serialize(sf::Packet &packet) const override { packet << x << y << z << dimension; }
	void deserialize(sf::Packet &packet) override { packet >> x >> y >> z >> dimension; }
	Network::Command packetType = Network::Command::EntityPosition;
};

#endif // POSITIONCOMPONENT_HPP_
