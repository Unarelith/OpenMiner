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
#ifndef EFFECTRENDERER_HPP_
#define EFFECTRENDERER_HPP_

#include <gk/gl/Shader.hpp>
#include <gk/gl/VertexBuffer.hpp>

#include "Framebuffer.hpp"

class EffectRenderer {
	public:
		EffectRenderer(u16 width, u16 height) { init(width, height); }

		void init(u16 width, u16 height);

		void loadShaders();

		void begin() const;
		void end() const;

		void render() const;

	private:
		void renderQuad() const;

		Framebuffer m_fbo{Framebuffer::All};

		Framebuffer m_pingpongBuffer[2]{Framebuffer::Color, Framebuffer::Color};

		gk::Shader m_shader;
		gk::Shader m_blurShader;

		gk::VertexBuffer m_vbo;
};

#endif // EFFECTRENDERER_HPP_
