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
#ifndef SKY_HPP_
#define SKY_HPP_

#include <string>

#include <gk/core/IntTypes.hpp>
#include <gk/core/ISerializable.hpp>
#include <gk/graphics/Color.hpp>

class Sky : public gk::ISerializable {
	public:
		Sky() = default;
		Sky(u16 id, const std::string &stringID);

		void serialize(sf::Packet &packet) const override;
		void deserialize(sf::Packet &packet) override;

		const std::string &stringID() const { return m_stringID; }

		const gk::Color &color() const { return m_color; }
		const gk::Color &fogColor() const { return m_fogColor; }

		void setColor(const gk::Color &color) { m_color = color; }
		void setFogColor(const gk::Color &fogColor) { m_fogColor = fogColor; }

	private:
		u16 m_id;
		std::string m_stringID;

		gk::Color m_color;
		gk::Color m_fogColor;
};

#endif // SKY_HPP_
