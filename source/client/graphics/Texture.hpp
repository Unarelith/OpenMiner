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
#ifndef TEXTURE_HPP_
#define TEXTURE_HPP_

#include <string>

#include <bgfx/bgfx.h>

#include "common/core/IntTypes.hpp"
#include "common/core/Vector2.hpp"
#include "common/utils/NonCopyable.hpp"

struct SDL_Surface;

class Texture : public NonCopyable {
	public:
		Texture() = default;
		Texture(const std::string &filename);
		Texture(SDL_Surface *surface);
		Texture(Texture &&texture);
		~Texture();

		Texture &operator=(Texture &&texture);

		void loadFromFile(const std::string &filename);
		void loadFromSurface(SDL_Surface *surface);
		void loadFromMemory(const bgfx::Memory *mem, int width, int height);
		void free();

		const std::string &filename() const { return m_filename; }

		const Vector2u &getSize() const { return m_size; }

		void enable(u8 unit, bgfx::UniformHandle handle) const;

	private:
		static const Texture *s_boundTexture;

		std::string m_filename;

		Vector2u m_size;

		bgfx::TextureHandle m_handle = BGFX_INVALID_HANDLE;
};

#endif // TEXTURE_HPP_
