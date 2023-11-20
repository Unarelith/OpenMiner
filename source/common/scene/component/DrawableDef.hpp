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
#ifndef DRAWABLEDEF_HPP_
#define DRAWABLEDEF_HPP_

#include "common/core/ISerializable.hpp"
#include "common/core/IntTypes.hpp"
#include "common/network/Network.hpp"
#include "common/scene/component/InventoryCubeDef.hpp"

namespace DrawableType {
	enum : u8 {
		Undefined     = 0,
		InventoryCube = 1,
	};
}

class DrawableDef : public ISerializable {
	public:
		InventoryCubeDef &addInventoryCube();

		// FIXME
		InventoryCubeDef &getInventoryCubeDef() { return m_cubes.back(); }
		const InventoryCubeDef &getInventoryCubeDef() const { return m_cubes.back(); }

		void serialize(sf::Packet &packet) const override;
		void deserialize(sf::Packet &packet) override;
		Network::Command packetType = Network::Command::EntityDrawableDef;

		bool isUpdated = true;

	protected:
		std::vector<InventoryCubeDef> m_cubes;
};

#endif // DRAWABLEDEF_HPP_
