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
#ifndef FRAMEBUFFER_HPP_
#define FRAMEBUFFER_HPP_

#include <SFML/Graphics/Texture.hpp>

#include <gk/core/IntTypes.hpp>
#include <gk/gl/OpenGL.hpp>
#include <gk/utils/NonCopyable.hpp>

class Framebuffer : public gk::NonCopyable {
	public:
		Framebuffer(u8 components) : m_components((Component)components) {}
		Framebuffer(u16 width, u16 height, u8 components);
		Framebuffer(Framebuffer &&) = default;
		~Framebuffer();

		Framebuffer &operator=(Framebuffer &&) = default;

		void init(u16 width, u16 height);
		void clear();

		void bindColorTexture(u16 index) const;
		void bindBloomTexture(u16 index) const;
		void bindDepthTexture(u16 index) const;

		static void bind(const Framebuffer *framebuffer);

		enum Component {
			Color = 1,
			Bloom = 2,
			Depth = 4,

			All   = Color | Bloom | Depth
		};

	private:
		Component m_components;

		GLuint m_id = 0;

		sf::Texture m_colorTex;
		sf::Texture m_bloomTex;
		GLuint m_depthTexID = 0;

		GLuint m_rbo = 0;
};

#endif // FRAMEBUFFER_HPP_
