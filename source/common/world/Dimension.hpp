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
#ifndef DIMENSION_HPP_
#define DIMENSION_HPP_

#include <string>
#include <vector>

#include <gk/core/IntTypes.hpp>

#include <sol.hpp>

#include "ISerializable.hpp"

class Dimension : public ISerializable {
	public:
		Dimension() = default;
		Dimension(u16 id, const std::string &stringID, const std::string &name)
			: m_id(id), m_stringID(stringID), m_name(name) {}

		u16 id() const { return m_id; }

		void addBiome(const std::string &biome) { m_biomes.emplace_back(biome); }

		void serialize(sf::Packet &packet) const override;
		void deserialize(sf::Packet &packet) override;

		const std::vector<std::string> &biomes() const { return m_biomes; }

		const std::string &sky() const { return m_sky; }
		void setSky(const std::string &sky) { m_sky = sky; }

		float gravity() const { return m_gravity; }
		void setGravity(float gravity) { m_gravity = gravity; }

		static void initUsertype(sol::state &lua);

	private:
		u16 m_id = 0;

		std::string m_stringID;
		std::string m_name;

		std::vector<std::string> m_biomes;

		std::string m_sky;

		float m_gravity = 1.f;
};

#endif // DIMENSION_HPP_
