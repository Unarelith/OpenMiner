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
#include <gk/core/Filesystem.hpp>
#include <gk/gl/OpenGL.hpp>

#include "Registry.hpp"
#include "TextureAtlas.hpp"

void TextureAtlas::clear() {
	m_tileSize = 0;

	m_textureMap.clear();
	m_textures.clear();

	m_isReady = false;
}

void TextureAtlas::loadFromRegistry(const std::string &texturePack) {
	if (!texturePack.empty() && !gk::Filesystem::fileExists("texturepacks/" + texturePack))
		throw EXCEPTION("Texture pack '" + texturePack +"' doesn't exist");

	// FIXME: Undefined texture should be created from current texture size
	if (texturePack.empty())
		addFile("mods/default/textures/blocks/", "undefined.png");
	else
		addFile("texturepacks/" + texturePack + "/blocks/", "undefined.png");

	for (auto &block : Registry::getInstance().blocks()) {
		std::string path;
		if (texturePack.empty())
			path = "mods/" + block->modName() + "/textures/blocks/";
		else
			path = "texturepacks/" + texturePack + "/blocks/";

		for (auto &state : block->states()) {
			const TilesDef &tiles = state.tiles();
			for (auto &textureFilename : tiles.textureFilenames())
				addFile(path, textureFilename);
		}
	}

	for (auto &item : Registry::getInstance().items()) {
		if (!item->isBlock() || !item->tiles().textureFilenames().empty()) {
			std::string path;
			if (texturePack.empty())
				path = "mods/" + item->modName() + "/textures/items/";
			else
				path = "texturepacks/" + texturePack + "/items/";

			const TilesDef &tiles = item->tiles();
			for (auto &textureFilename : tiles.textureFilenames())
				addFile(path, textureFilename);
		}
	}

	packTextures();
}

gk::FloatRect TextureAtlas::getTexCoords(const std::string &filename, bool normalized) const {
	if (filename.empty()) return gk::FloatRect{0, 0, 0, 0};

	if (!m_isReady)
		throw EXCEPTION("Can't get texture coordinates from empty atlas");

	u16 textureID = getTextureID(filename);

	float textureX = float((textureID % (m_texture.getSize().x / m_tileSize)) * m_tileSize);
	float textureY = float((textureID / (m_texture.getSize().x / m_tileSize)) * m_tileSize);

	if (normalized)
		return gk::FloatRect{textureX / (float)m_texture.getSize().x,
							 textureY / (float)m_texture.getSize().y,
							 m_tileSize / (float)m_texture.getSize().x,
							 m_tileSize / (float)m_texture.getSize().y};
	else
		return gk::FloatRect{textureX,
							 textureY,
							 (float)m_tileSize,
							 (float)m_tileSize};
}

void TextureAtlas::addFile(const std::string &path, const std::string &filename) {
	if (filename.empty()) return;

	auto it = m_textureMap.find(filename);
	if (it != m_textureMap.end())
		return;

	SurfacePtr surface{IMG_Load((path + filename).c_str()), &SDL_FreeSurface};
	if(!surface) {
		gkError() << "Failed to load texture:" << path + filename;
		return;
	}

	if (!m_tileSize)
		m_tileSize = (u16)surface->w;

	if (m_tileSize != surface->w || m_tileSize != surface->h)
		throw EXCEPTION("Texture size unexpected for", path + filename + ". Got", surface->w, surface->h, "instead of", m_tileSize, m_tileSize);

	m_textureMap.emplace(filename, (u16)m_textures.size());
	m_textures.emplace_back(std::move(surface));
}

u16 TextureAtlas::getTextureID(const std::string &filename) const {
	auto it = m_textureMap.find(filename);
	if (it == m_textureMap.end()) {
		return 0;
	}

	return it->second;
}

void TextureAtlas::packTextures() {
	if (!m_tileSize)
		throw EXCEPTION("Cannot pack zero-sized textures!");

	// Max amount of textures on one line
	const u16 atlasWidth = 16;

	// Max amount of textures on one column
	const u16 atlasHeight = (u16)std::ceil((float)m_textures.size() / atlasWidth);

	SurfacePtr atlas{nullptr, &SDL_FreeSurface};

	Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
#else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;
#endif

	atlas.reset(SDL_CreateRGBSurface(0, atlasWidth * m_tileSize, atlasHeight * m_tileSize, 32, rmask, gmask, bmask, amask));
	if (!atlas) {
		throw EXCEPTION("Failed to create surface:", SDL_GetError());
	}

	u16 i = 0;
	for (auto &it : m_textures) {
		SDL_Rect outRect;
		outRect.x = (i % atlasWidth) * m_tileSize;
		outRect.y = (i / atlasWidth) * m_tileSize;
		outRect.w = m_tileSize;
		outRect.h = m_tileSize;

		SDL_BlitSurface(it.get(), nullptr, atlas.get(), &outRect);

		++i;
	}

	m_textures.clear();

	m_isReady = true;

	if (IMG_SavePNG(atlas.get(), "test_atlas.png") < 0)
		throw EXCEPTION("Failed to save texture to: test_atlas.png. Reason:", IMG_GetError());

	m_texture.loadFromSurface(atlas.get());

	gk::Texture::bind(&m_texture);

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	gk::Texture::bind(nullptr);
}
