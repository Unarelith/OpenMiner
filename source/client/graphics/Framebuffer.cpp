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
#include "common/core/Exception.hpp"
#include "client/core/GameConfig.hpp"
#include "client/graphics/BgfxView.hpp"
#include "client/graphics/Framebuffer.hpp"

Framebuffer::Framebuffer() {
	const float minv = (bgfx::getCaps()->originBottomLeft) ? 0.0f : 1.0f;
	const float maxv = (bgfx::getCaps()->originBottomLeft) ? 1.0f : 0.0f;

	float quad[24] = {
		-1.0f,  1.0f,  0.0f, maxv,
		-1.0f, -1.0f,  0.0f, minv,
		 1.0f, -1.0f,  1.0f, minv,

		-1.0f,  1.0f,  0.0f, maxv,
		 1.0f, -1.0f,  1.0f, minv,
		 1.0f,  1.0f,  1.0f, maxv,
	};

	m_vbo.layout().begin()
		.add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
		.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
		.end();

	m_vbo.init(quad, sizeof(quad));

	m_colorTextureSampler.init("s_colorTexture", bgfx::UniformType::Sampler);
	m_depthTextureSampler.init("s_depthTexture", bgfx::UniformType::Sampler);

	m_effectTypeUniform.init("u_effectType", bgfx::UniformType::Vec4);
	m_depthFogColorUniform.init("u_depthFogColor", bgfx::UniformType::Vec4);
}

Framebuffer::Framebuffer(u16 width, u16 height) : Framebuffer() {
	init(width, height);
}

Framebuffer::~Framebuffer() {
	free();
}

void Framebuffer::init(u16 width, u16 height) {
	const uint64_t colorTextureFlags = BGFX_TEXTURE_RT | BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP;
	const uint64_t depthTextureFlags = BGFX_TEXTURE_RT;

	m_textures[0] = bgfx::createTexture2D(width, height, false, 1, bgfx::TextureFormat::BGRA8, colorTextureFlags);
	m_textures[1] = bgfx::createTexture2D(width, height, false, 1, bgfx::TextureFormat::D24S8, depthTextureFlags);

	if (!bgfx::isValid(m_textures[0]))
		throw EXCEPTION("Framebuffer error: Unable to create color texture with format BGRA8");
	if (!bgfx::isValid(m_textures[1]))
		throw EXCEPTION("Framebuffer error: Unable to create depth texture with format D24S8");

	if (bgfx::isValid(m_handle))
		bgfx::destroy(m_handle);

	m_handle = bgfx::createFrameBuffer(2, m_textures, true);

	if (!bgfx::isValid(m_handle))
		throw EXCEPTION("Failed to create framebuffer");

	bgfx::setViewRect(BgfxView::Effect, 0, 0, width, height);
	bgfx::setViewClear(BgfxView::Effect, BGFX_CLEAR_COLOR);
}

void Framebuffer::free() {
	if (bgfx::isValid(m_handle)) {
		bgfx::destroy(m_handle);
		m_handle.idx = bgfx::kInvalidHandle;
	}
}

void Framebuffer::loadShader(const std::string &name) {
	m_shader.loadFromFile(name);
}

void Framebuffer::prepareDraw() const {
	bgfx::setViewFrameBuffer(BgfxView::Sky, m_handle);
	bgfx::setViewFrameBuffer(BgfxView::World, m_handle);
}

void Framebuffer::draw() const {
	bgfx::setTexture(0, m_colorTextureSampler.handle(), m_textures[0]);
	bgfx::setTexture(1, m_depthTextureSampler.handle(), m_textures[1]);

	m_effectTypeUniform.setValue(GameConfig::currentScreenEffect, GameConfig::fogDepth);

	m_depthFogColorUniform.setValue(GameConfig::fogColor);

	m_vbo.enable();

	bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A);

	bgfx::submit(BgfxView::Effect, m_shader.program());
}
