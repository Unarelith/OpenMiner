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

#include "client/graphics/Shader.hpp"
#include "client/graphics/ShaderUniform.hpp"
#include "client/graphics/Texture.hpp"
#include "client/graphics/VertexBuffer.hpp"

class Framebuffer : public NonCopyable {
	public:
		Framebuffer();
		Framebuffer(u16 width, u16 height);
		Framebuffer(Framebuffer &&) = default;
		~Framebuffer();

		Framebuffer &operator=(Framebuffer &&) = default;

		void init(u16 width, u16 height);
		void free();

		void loadShader(const std::string &name);

		void prepareDraw() const;
		void draw() const;

	private:
		ShaderUniform m_colorTextureSampler;
		ShaderUniform m_depthTextureSampler;

		ShaderUniform m_effectTypeUniform;
		ShaderUniform m_depthFogColorUniform;

		bgfx::TextureHandle m_textures[2] = {BGFX_INVALID_HANDLE, BGFX_STATE_BLEND_INV_SRC_ALPHA};

		bgfx::FrameBufferHandle m_handle = BGFX_INVALID_HANDLE;

		Shader m_shader;
		VertexBuffer m_vbo;
};

#endif // FRAMEBUFFER_HPP_
