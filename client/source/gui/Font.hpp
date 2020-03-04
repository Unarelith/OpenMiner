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
#ifndef FONT_HPP_
#define FONT_HPP_

#include <string>

#include <gk/core/Vector2.hpp>

namespace gk {
	class Texture;
}

class Font {
	public:
		Font(const std::string &textureName, const std::string &configPath);

		gk::Vector2f getTexCoords(u8 c, u8 x, u8 y) const;

		u8 getCharWidth(u8 c) const { return m_charWidth[c]; }
		gk::Vector2i getTileSize() const { return {m_width, m_height}; }

		const std::string &textureName() const { return m_textureName; } // FIXME: Will be removed later
		const gk::Texture &texture() const { return m_texture; }

	private:
		void parseConfig(const std::string &configPath);

		std::string m_textureName; // FIXME: Will be removed later
		gk::Texture &m_texture;

		u8 m_charWidth[256];
		u8 m_width = 8; // FIXME: Hardcoded value
		u8 m_height = 8; // FIXME: Hardcoded value
};

#endif // FONT_HPP_
