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

#include <gk/core/IntTypes.hpp>
#include <gk/core/Rect.hpp>
#include <gk/gl/Texture.hpp>

class TextureAtlas {
	public:
		TextureAtlas() = default;

		void clear();

		void loadFromRegistry(const std::string &texturePack = "");

		gk::FloatRect getTexCoords(const std::string &filename, bool normalized = true) const;

		const gk::Texture &texture() const { return m_texture; }

		bool isReady() const { return m_isReady; }

	private:
		void addFile(const std::string &path, const std::string &filename);

		u16 getTextureID(const std::string &filename) const;

		void packTextures();

		// Expected texture size
		u16 m_tileSize = 0;

		// Mapping between filename and internal texture ID
		std::unordered_map<std::string, u16> m_textureMap;

		// Textures to pack together
		using SurfacePtr = std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)>;
		std::vector<SurfacePtr> m_textures;

		// Packed texture
		gk::Texture m_texture;

		// Is the texture atlas ready to use?
		bool m_isReady = false;
};

#endif // TEXTUREATLAS_HPP_
