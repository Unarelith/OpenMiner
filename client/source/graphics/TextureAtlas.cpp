/*
 * =====================================================================================
 *
 *       Filename:  TextureAtlas.cpp
 *
 *    Description:
 *
 *        Created:  29/01/2020 16:34:43
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include <gk/core/Exception.hpp>

#include "TextureAtlas.hpp"

void TextureAtlas::addFile(const std::string &filename) {
	SurfacePtr surface{IMG_Load(filename.c_str()), &SDL_FreeSurface};
	if(!surface) {
		throw EXCEPTION("Failed to load texture:", filename);
	}

	auto it = m_textures.find(filename);
	if (it != m_textures.end()) {
		throw EXCEPTION("Texture already exists in atlas:", filename);
	}

	if (!m_width && !m_height) {
		m_width = surface->w;
		m_height = surface->h;
	}
	else if (m_width != surface->w || m_height != surface->h) {
		throw EXCEPTION("Texture size unexpected for", filename + ". Got", surface->w, surface->w, "instead of", m_width, m_height);
	}

	m_textures.emplace(filename, std::move(surface));
}

void TextureAtlas::packTextures() {
	if (!m_width && !m_height) {
		throw EXCEPTION("Cannot pack zero-sized textures!");
	}

	// Max amount of textures on one line
	const u16 atlasWidth = 16;

	// Max amount of textures on one column
	const u16 atlasHeight = m_textures.size() / atlasWidth;

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

	m_atlas.reset(SDL_CreateRGBSurface(0, atlasWidth * m_width, atlasHeight * m_height, 32, rmask, gmask, bmask, amask));
	if (!m_atlas) {
		throw EXCEPTION("Failed to create surface:", SDL_GetError());
	}

	u16 i = 0;
	for (auto &it : m_textures) {
		SDL_Rect outRect;
		outRect.x = (i % atlasWidth) * m_width;
		outRect.y = (i / atlasWidth) * m_height;
		outRect.w = m_width;
		outRect.h = m_height;

		SDL_BlitSurface(it.second.get(), nullptr, m_atlas.get(), &outRect);

		m_textureMap.emplace(it.first, i);

		++i;
	}

	m_textures.clear();
}

void TextureAtlas::saveToFile(const std::string &filename) {
	if (IMG_SavePNG(m_atlas.get(), filename.c_str()) < 0) {
		throw EXCEPTION("Failed to save texture to:", filename + ". Reason:", SDL_GetError());
	}
}

u16 TextureAtlas::getTextureID(const std::string &filename) {
	auto it = m_textureMap.find(filename);
	if (it == m_textureMap.end()) {
		throw EXCEPTION("Unable to find texture in atlas:", filename);
	}

	return it->second;
}

