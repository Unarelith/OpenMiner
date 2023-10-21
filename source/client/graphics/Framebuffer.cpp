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

#include "Framebuffer.hpp"
#include "GameConfig.hpp"

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

	m_colorTextureSampler = bgfx::createUniform("s_colorTexture", bgfx::UniformType::Sampler);
	m_depthTextureSampler = bgfx::createUniform("s_depthTexture", bgfx::UniformType::Sampler);

	m_effectTypeUniform = bgfx::createUniform("u_effectType", bgfx::UniformType::Vec4);
	m_depthFogColorUniform = bgfx::createUniform("u_depthFogColor", bgfx::UniformType::Vec4);
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

	m_handle = bgfx::createFrameBuffer(2, m_textures, true);

	if (!bgfx::isValid(m_handle))
		throw EXCEPTION("Failed to create framebuffer");

	bgfx::setViewRect(view, 0, 0, width, height);
	bgfx::setViewClear(view, BGFX_CLEAR_COLOR);

	bgfx::setViewFrameBuffer(0, m_handle);
	bgfx::setViewFrameBuffer(1, m_handle);
}

void Framebuffer::free() {
	bgfx::setViewFrameBuffer(0, BGFX_INVALID_HANDLE);
	bgfx::setViewFrameBuffer(1, BGFX_INVALID_HANDLE);

	if (bgfx::isValid(m_handle)) {
		bgfx::destroy(m_handle);
		m_handle.idx = bgfx::kInvalidHandle;
	}

	if (bgfx::isValid(m_depthFogColorUniform)) {
		bgfx::destroy(m_depthFogColorUniform);
		m_depthFogColorUniform.idx = bgfx::kInvalidHandle;
	}

	if (bgfx::isValid(m_effectTypeUniform)) {
		bgfx::destroy(m_effectTypeUniform);
		m_effectTypeUniform.idx = bgfx::kInvalidHandle;
	}

	if (bgfx::isValid(m_depthTextureSampler)) {
		bgfx::destroy(m_depthTextureSampler);
		m_depthTextureSampler.idx = bgfx::kInvalidHandle;
	}

	if (bgfx::isValid(m_colorTextureSampler)) {
		bgfx::destroy(m_colorTextureSampler);
		m_colorTextureSampler.idx = bgfx::kInvalidHandle;
	}
}

void Framebuffer::loadShader(const std::string &name) {
	m_shader.loadFromFile(name);
}

void Framebuffer::draw() const {
	bgfx::setTexture(0, m_colorTextureSampler, m_textures[0]);
	bgfx::setTexture(1, m_depthTextureSampler, m_textures[1]);

	float effectType[4] = {(float)GameConfig::currentScreenEffect, GameConfig::fogDepth, 0.f, 0.f};
	bgfx::setUniform(m_effectTypeUniform, effectType);

	float fogColor[4] = {
		GameConfig::fogColor.r,
		GameConfig::fogColor.g,
		GameConfig::fogColor.b,
		GameConfig::fogColor.a,
	};
	bgfx::setUniform(m_depthFogColorUniform, fogColor);

	m_vbo.enable();

	bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A);

	bgfx::submit(view, m_shader.program());
}
