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

Framebuffer::Framebuffer(u16 width, u16 height) {
	init(width, height);
}

Framebuffer::~Framebuffer() {
	clear();
}

void Framebuffer::init(u16 width, u16 height) {
	if (m_id)
		clear();

	glCheck(glGenFramebuffers(1, &m_id));
	bind(this);

	glCheck(glGenTextures(1, &m_texid));
	glCheck(glBindTexture(GL_TEXTURE_2D, m_texid));
	glCheck(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr));
	glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	glCheck(glBindTexture(GL_TEXTURE_2D, 0));

	glCheck(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texid, 0));

	glCheck(glGenRenderbuffers(1, &m_rbo));
	glCheck(glBindRenderbuffer(GL_RENDERBUFFER, m_rbo));
	glCheck(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height));
	glCheck(glBindRenderbuffer(GL_RENDERBUFFER, 0));

	glCheck(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo));

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		throw EXCEPTION("Framebuffer is not complete");

	bind(nullptr);
}

void Framebuffer::clear() {
	if (m_id) {
		glCheck(glDeleteFramebuffers(1, &m_id));
		m_id = 0;
	}

	if (m_texid) {
		glCheck(glDeleteTextures(1, &m_texid));
		m_texid = 0;
	}

	if (m_rbo) {
		glCheck(glDeleteRenderbuffers(1, &m_rbo));
		m_rbo = 0;
	}
}

void Framebuffer::bind(const Framebuffer *framebuffer) {
	if (framebuffer)
		glCheck(glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->m_id));
	else
		glCheck(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

