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
#ifndef TILESDEF_HPP_
#define TILESDEF_HPP_

#include <string>
#include <vector>

#include <SFML/Network/Packet.hpp>

#include <gk/core/IntTypes.hpp>
#include <gk/core/ISerializable.hpp>

#include <sol/sol.hpp>

enum BlockFace : u8 {
	West   = 0,
	East   = 1,
	South  = 2,
	North  = 3,
	Bottom = 4,
	Top    = 5,
};

class TilesDef : public gk::ISerializable {
	public:
		TilesDef() = default;
		TilesDef(const std::string &objectID, u16 stateID)
			: m_objectID(objectID), m_stateID(stateID) {}
		TilesDef(const std::string &objectID, u16 stateID, const std::string &texture) : TilesDef(objectID, stateID) {
			m_textureFilenames.emplace_back(texture);
		}

		const std::string &getTextureForFace(u8 face, bool useAltTiles = false) const;

		void serialize(sf::Packet &packet) const override;
		void deserialize(sf::Packet &packet) override;

		bool loadFromLuaTable(const sol::table &table);

		const std::vector<std::string> &textureFilenames() const { return m_textureFilenames; };
		const std::vector<std::string> &altTextureFilenames() const { return m_altTextureFilenames; }

	private:
		std::string m_objectID;
		u16 m_stateID = 0;

		std::vector<std::string> m_textureFilenames;
		std::vector<std::string> m_altTextureFilenames;
};

#endif // TILESDEF_HPP_
