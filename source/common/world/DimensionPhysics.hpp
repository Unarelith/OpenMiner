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
#ifndef DIMENSIONPHYSICS_HPP_
#define DIMENSIONPHYSICS_HPP_

#include <gk/core/ISerializable.hpp>

struct DimensionPhysics : public gk::ISerializable {
	void serialize(sf::Packet &packet) const override;
	void deserialize(sf::Packet &packet) override;
		
	float gravity = 1.f;
	float jumpSpeed = 0.05f;
	
	float jumpAntigravity = 0.3f;
	float glideGravity = 0.04f;

	float horizontalSprintMod = 1.5f;
	float verticalSprintMod = 1.5f;

	float moveSpeed = 0.04f;
	float airSpeedMod = 0.75f;

	float flySpeed = 0.1f;
	float sneakVerticalSpeed = 0.1f;
	float sneakHorizontalMod = 0.5f;

	bool isSneakAlwaysMod = false;
};

#endif // DIMENSIONPHYSICS_HPP_