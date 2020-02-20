/*
 * =====================================================================================
 *
 *  OpenMiner
 *  Copyright (C) 2018-2020 Unarelith, Quentin Bazin <openminer@unarelith.net>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program; if not, write to the Free Software Foundation, Inc.,
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

	u8 outFace = face;

	if (size == 1)
		outFace = BlockFace::Top;
	else if (size == 2)
		outFace = (face == BlockFace::Top || face == BlockFace::Bottom) ? 0 : 1;
	else if (size == 3 && face >= size)
		outFace = BlockFace::West;
	else if (size == 4 && face >= size)
		outFace = BlockFace::East;
	else if (size == 5 && face >= size)
		outFace = BlockFace::South;

	if (!useAltTiles)
		return m_textureFilenames.at(outFace);
	else {
		const std::string &filename = m_altTextureFilenames.at(outFace);
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
		sol::as_table_t<std::vector<std::string>> t = table["tiles"];
		m_textureFilenames = t.source;
	}
	else
		m_textureFilenames.emplace_back(table["tiles"].get<std::string>());

	if (table["alt_tiles"].get_type() != sol::type::none) {
		if (table["alt_tiles"].get_type() == sol::type::table) {
			sol::as_table_t<std::vector<std::string>> t = table["alt_tiles"];
			m_altTextureFilenames = t.source;
		}
		else
			m_altTextureFilenames.emplace_back(table["alt_tiles"].get<std::string>());
	}
}

