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
#ifndef TEXTUREATLAS_HPP_
#define TEXTUREATLAS_HPP_

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <SFML/Graphics/Texture.hpp>

#include <gk/core/IntTypes.hpp>
#include <gk/core/Rect.hpp>

class TextureAtlas {
	public:
		TextureAtlas() = default;

		void addFile(const std::string &path, const std::string &filename);
		void packTextures();

		void loadFromRegistry(const std::string &texturePack = "");

		u16 getTextureID(const std::string &filename) const;
		gk::FloatRect getTexCoords(const std::string &filename, bool normalized = true) const;

		const sf::Texture &texture() const { return m_texture; }

		bool isReady() const { return m_isReady; }

	private:
		// Expected texture size
		u16 m_tileSize = 0;

		// Mapping between filename and internal texture ID
		std::unordered_map<std::string, u16> m_textureMap;

		// Packed texture
		sf::Texture m_texture;

		// Is the texture atlas ready to use?
		bool m_isReady = false;
};

#endif // TEXTUREATLAS_HPP_
