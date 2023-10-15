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
#include <cassert>

#include <gk/core/Exception.hpp>

#include "Texture.hpp"

const Texture *Texture::s_boundTexture = nullptr;

Texture::Texture(const std::string &filename) {
	loadFromFile(filename);
}

Texture::Texture(SDL_Surface *surface) {
	loadFromSurface(surface);
}

Texture::Texture(Texture &&texture) {
	m_filename = texture.m_filename;

	m_size = texture.m_size;

	m_handle = texture.m_handle;
	texture.m_handle.idx = bgfx::kInvalidHandle;
}

Texture::~Texture() {
	if (bgfx::isValid(m_handle))
		bgfx::destroy(m_handle);
}

Texture &Texture::operator=(Texture &&texture) {
	m_filename = texture.m_filename;

	m_size = texture.m_size;

	m_handle = texture.m_handle;
	texture.m_handle.idx = bgfx::kInvalidHandle;

	return *this;
}

void Texture::loadFromFile(const std::string &filename) {
	m_filename = filename;

	SDL_Surface *surface = IMG_Load(m_filename.c_str());
	if(!surface) {
		throw EXCEPTION("Failed to load texture:", filename);
	}

	loadFromSurface(surface);

	SDL_FreeSurface(surface);
}

void Texture::loadFromSurface(SDL_Surface *surface) {
	m_size.x = surface->w;
	m_size.y = surface->h;

	assert(!bgfx::isValid(m_handle));

	bgfx::TextureFormat::Enum format;
	if (surface->format->BytesPerPixel == 4) {
		format = (surface->format->Rmask == 0x000000ff) ? bgfx::TextureFormat::RGBA8 : bgfx::TextureFormat::BGRA8;
	}
	else if (surface->format->BytesPerPixel == 3) {
		format = (surface->format->Rmask == 0x000000ff) ? bgfx::TextureFormat::RGB8 : bgfx::TextureFormat::BGRA8;
	}
	else {
		gkError() << ("Unable to load '" + m_filename + "': The image file is not using true colors").c_str();
		return;
	}

	u64 flags = BGFX_TEXTURE_NONE
	          | BGFX_SAMPLER_MIN_POINT
	          | BGFX_SAMPLER_MAG_POINT;

	u32 size = m_size.y * surface->pitch;
	m_handle = bgfx::createTexture2D((u16)m_size.x, (u16)m_size.y, false, 1, format, flags, bgfx::copy(surface->pixels, size));
}

void Texture::enable(u8 unit, bgfx::UniformHandle handle) const {
	bgfx::setTexture(unit, handle, m_handle);
}
