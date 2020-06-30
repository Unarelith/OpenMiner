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
#include <gk/gl/GLCheck.hpp>

#include "Framebuffer.hpp"

Framebuffer::Framebuffer(u16 width, u16 height, u8 components) : m_components((Component)components) {
	init(width, height);
}

Framebuffer::~Framebuffer() {
	clear();
}

void Framebuffer::init(u16 width, u16 height) {
	if (m_id)
		clear();

	// Create and bind our framebuffer
	glCheck(glGenFramebuffers(1, &m_id));
	bind(this);

	// Create a texture to store the colors
	if (m_components & Component::Color) {
		m_colorTex.setSmooth(true);
		m_colorTex.create(width, height);

		glCheck(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorTex.getNativeHandle(), 0));
	}

	// Create texture for bloom effect
	if (m_components & Component::Bloom) {
		m_bloomTex.setSmooth(true);
		m_bloomTex.create(width, height);

		glCheck(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_bloomTex.getNativeHandle(), 0));
	}

	if (m_components & Component::Depth) {
		// Create a texture to store depth data
		glCheck(glGenTextures(1, &m_depthTexID));
		glCheck(glBindTexture(GL_TEXTURE_2D, m_depthTexID));
		glCheck(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, nullptr));
		glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		glCheck(glBindTexture(GL_TEXTURE_2D, 0));

		glCheck(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexID, 0));
	}
	else {
		// Create a renderbuffer
		glCheck(glGenRenderbuffers(1, &m_rbo));
		glCheck(glBindRenderbuffer(GL_RENDERBUFFER, m_rbo));
		glCheck(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height));
		glCheck(glBindRenderbuffer(GL_RENDERBUFFER, 0));

		glCheck(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo));
	}

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		throw EXCEPTION("Framebuffer is not complete");

	bind(nullptr);
}

void Framebuffer::clear() {
	if (m_id) {
		glCheck(glDeleteFramebuffers(1, &m_id));
		m_id = 0;
	}

	if (m_depthTexID) {
		glCheck(glDeleteTextures(1, &m_depthTexID));
		m_depthTexID = 0;
	}

	if (m_rbo) {
		glCheck(glDeleteRenderbuffers(1, &m_rbo));
		m_rbo = 0;
	}
}

#include <gk/core/GameClock.hpp>

void Framebuffer::bindColorTexture(u16 index) const {
	if (m_colorTex.getNativeHandle() != 0) {
		glActiveTexture(GL_TEXTURE0 + index);
		sf::Texture::bind(&m_colorTex);

		// if (gk::GameClock::getInstance().getTicks() % 180 == 0)
		// 	m_colorTex.copyToImage().saveToFile("tex_color" + std::to_string(m_id) + "-" + std::to_string(index) + "_test.png");
	}
}

void Framebuffer::bindBloomTexture(u16 index) const {
	if (m_bloomTex.getNativeHandle() != 0) {
		glActiveTexture(GL_TEXTURE0 + index);
		sf::Texture::bind(&m_bloomTex);

		// if (gk::GameClock::getInstance().getTicks() % 180 == 0)
		// 	m_bloomTex.copyToImage().saveToFile("tex_bloom" + std::to_string(m_id) + "-" + std::to_string(index) + "_test.png");
	}
}

void Framebuffer::bindDepthTexture(u16 index) const {
	if (m_depthTexID != 0) {
		glActiveTexture(GL_TEXTURE0 + index);
		glBindTexture(GL_TEXTURE_2D, m_depthTexID);
	}
}

void Framebuffer::bind(const Framebuffer *framebuffer) {
	if (framebuffer)
		glCheck(glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->m_id));
	else
		glCheck(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

