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
#include <gk/core/Exception.hpp>

#include "NetworkUtils.hpp"
#include "TilesDef.hpp"

const std::string &TilesDef::getTextureForFace(u8 face, bool useAltTiles) const {
	u8 size = (!useAltTiles) ? m_textureFilenames.size() : m_altTextureFilenames.size();
	if (size == 0)
		throw EXCEPTION("Trying to get texture from empty tiles definition");

	static constexpr int nSizes = 6, nFaces = 6;
	// Determine which indices correspond to each face, depending on how many
	// textures were specified
	// NOTE: If this table changes, please change 'docs/lua-api-blocks.md' accordingly
	static constexpr u8 faceToIndex[nSizes][nFaces] = {
		// Same order as enum BlockFace in TilesDef.hpp,
		// namely: West, East, South, North, Bottom, Top
		{0, 0, 0, 0, 0, 0}, // for size = 1
		{1, 1, 1, 1, 0, 0}, // for size = 2
		{2, 2, 2, 2, 1, 0}, // for size = 3
		{3, 2, 3, 3, 1, 0}, // for size = 4
		{3, 2, 4, 4, 1, 0}, // for size = 5
		{3, 2, 4, 5, 1, 0}, // for size = 6
	};

	u8 index = faceToIndex[size <= nSizes ? size - 1 : nSizes - 1][face];

	if (!useAltTiles)
		return m_textureFilenames.at(index);
	else {
		const std::string &filename = m_altTextureFilenames.at(index);
		return (!filename.empty()) ? filename : getTextureForFace(face, false);
	}
}

void TilesDef::serialize(sf::Packet &packet) const {
	packet << m_textureFilenames << m_altTextureFilenames;
}

void TilesDef::deserialize(sf::Packet &packet) {
	packet >> m_textureFilenames >> m_altTextureFilenames;
}

void TilesDef::loadFromLuaTable(const sol::table &table) {
	if (table["tiles"].get_type() == sol::type::table) {
		m_textureFilenames = table["tiles"].get<std::vector<std::string>>();
	}
	else
		m_textureFilenames.emplace_back(table["tiles"].get<std::string>());

	if (table["alt_tiles"].get_type() != sol::type::none) {
		if (table["alt_tiles"].get_type() == sol::type::table) {
			m_altTextureFilenames = table["alt_tiles"].get<std::vector<std::string>>();
		}
		else
			m_altTextureFilenames.emplace_back(table["alt_tiles"].get<std::string>());
	}
}

