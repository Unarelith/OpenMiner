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
#include "GameConfig.hpp"

Framebuffer::Framebuffer(u16 width, u16 height) {
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
	glCheck(glGenTextures(1, &m_colorTexID));
	glCheck(glBindTexture(GL_TEXTURE_2D, m_colorTexID));
	glCheck(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr));
	glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	glCheck(glBindTexture(GL_TEXTURE_2D, 0));

	glCheck(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorTexID, 0));

	// Create a renderbuffer
	// glCheck(glGenRenderbuffers(1, &m_rbo));
	// glCheck(glBindRenderbuffer(GL_RENDERBUFFER, m_rbo));
	// glCheck(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height));
	// glCheck(glBindRenderbuffer(GL_RENDERBUFFER, 0));
    //
	// glCheck(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo));

	// Create a texture to store depth data
	glCheck(glGenTextures(1, &m_depthTexID));
	glCheck(glBindTexture(GL_TEXTURE_2D, m_depthTexID));
	glCheck(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, nullptr));
	glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	glCheck(glBindTexture(GL_TEXTURE_2D, 0));

	glCheck(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexID, 0));

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		throw EXCEPTION("Framebuffer is not complete");

	bind(nullptr);

	float quad[24] = {
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	gk::VertexBuffer::bind(&m_vbo);
	m_vbo.setData(sizeof(quad), &quad, GL_STATIC_DRAW);
	gk::VertexBuffer::bind(nullptr);
}

void Framebuffer::clear() {
	if (m_id) {
		glCheck(glDeleteFramebuffers(1, &m_id));
		m_id = 0;
	}

	if (m_colorTexID) {
		glCheck(glDeleteTextures(1, &m_colorTexID));
		m_colorTexID = 0;
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

void Framebuffer::loadShader(const std::string &name) {
	m_shader.createProgram();
	m_shader.addShader(GL_VERTEX_SHADER, "resources/shaders/" + name + ".v.glsl");
	m_shader.addShader(GL_FRAGMENT_SHADER, "resources/shaders/" + name + ".f.glsl");
	m_shader.linkProgram();

	gk::Shader::bind(&m_shader);
	m_shader.setUniform("screenTexture", 0);
	m_shader.setUniform("depthTexture", 1);
	gk::Shader::bind(nullptr);
}

void Framebuffer::begin() const {
	bind(this);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
}

void Framebuffer::end() const {
	bind(nullptr);
	glClear(GL_COLOR_BUFFER_BIT);

	glDisable(GL_DEPTH_TEST);

	if (m_colorTexID != 0) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_colorTexID);
	}

	if (m_depthTexID != 0) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_depthTexID);
	}

	gk::Shader::bind(&m_shader);
	m_shader.setUniform("u_effectType", GameConfig::currentScreenEffect);
	m_shader.setUniform("u_fogDepth", GameConfig::fogDepth);
	m_shader.setUniform("u_fogColor", GameConfig::fogColor);

	gk::VertexBuffer::bind(&m_vbo);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	m_vbo.setAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(0 * sizeof(float)));
	m_vbo.setAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	gk::VertexBuffer::bind(nullptr);
	gk::Shader::bind(nullptr);

	glActiveTexture(GL_TEXTURE0);

	glEnable(GL_DEPTH_TEST);
}

void Framebuffer::bind(const Framebuffer *framebuffer) {
	if (framebuffer)
		glCheck(glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->m_id));
	else
		glCheck(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

