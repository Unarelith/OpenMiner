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

void TextureAtlas::addFile(const std::string &path, const std::string &filename) {
	if (filename.empty()) return;

	auto it = m_textureMap.find(filename);
	if (it != m_textureMap.end())
		return;

	sf::Image image;
	if (!image.loadFromFile(path + filename)) {
		gkWarning() << "Failed to load texture:" << path + filename;
		return;
	}

	if (!m_tileSize)
		m_tileSize = image.getSize().x;

	if (m_tileSize != image.getSize().x || m_tileSize != image.getSize().y)
		throw EXCEPTION("Texture size unexpected for", path + filename + ". Got", image.getSize().x, image.getSize().y, "instead of", m_tileSize, m_tileSize);

	m_textureMap.emplace(filename, m_images.size());
	m_images.emplace_back(std::move(image));
}

void TextureAtlas::packTextures() {
	if (!m_tileSize)
		throw EXCEPTION("Cannot pack zero-sized textures!");

	// Max amount of textures on one line
	const u16 atlasWidth = 16;

	// Max amount of textures on one column
	const u16 atlasHeight = std::ceil((float)m_images.size() / atlasWidth);

	sf::Image atlas;
	atlas.create(atlasWidth * m_tileSize, atlasHeight * m_tileSize);

	u16 i = 0;
	for (auto &it : m_images) {
		atlas.copy(it, (i % atlasWidth) * m_tileSize, (i / atlasWidth) * m_tileSize);
		++i;
	}

	m_images.clear();

	m_isReady = true;

	if (!atlas.saveToFile("test_atlas.png"))
		throw EXCEPTION("Failed to save texture atlas to: test_atlas.png.");

	m_texture.loadFromImage(atlas);

	sf::Texture::bind(&m_texture);

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	sf::Texture::bind(nullptr);
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

		const TilesDef &tiles = block->tiles();
		for (auto &textureFilename : tiles.textureFilenames())
			addFile(path, textureFilename);
		for (auto &textureFilename : tiles.altTextureFilenames())
			addFile(path, textureFilename);
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
			for (auto &textureFilename : tiles.altTextureFilenames())
				addFile(path, textureFilename);
		}
	}

	packTextures();
}

u16 TextureAtlas::getTextureID(const std::string &filename) const {
	auto it = m_textureMap.find(filename);
	if (it == m_textureMap.end()) {
		return 0;
	}

	return it->second;
}

gk::FloatRect TextureAtlas::getTexCoords(const std::string &filename, bool normalized) const {
	if (filename.empty()) return gk::FloatRect{0, 0, 0, 0};

	if (!m_isReady)
		throw EXCEPTION("Can't get texture coordinates from empty atlas");

	u16 textureID = getTextureID(filename);

	float textureX = (textureID % (m_texture.getSize().x / m_tileSize)) * m_tileSize;
	float textureY = (textureID / (m_texture.getSize().x / m_tileSize)) * m_tileSize;

	if (normalized)
		return gk::FloatRect{textureX / m_texture.getSize().x,
							 textureY / m_texture.getSize().y,
							 (float)m_tileSize / m_texture.getSize().x,
							 (float)m_tileSize / m_texture.getSize().y};
	else
		return gk::FloatRect{textureX,
							 textureY,
							 (float)m_tileSize,
							 (float)m_tileSize};
}

