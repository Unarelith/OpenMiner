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
#include "Config.hpp"
#include "EffectRenderer.hpp"

void EffectRenderer::init(u16 width, u16 height) {
	m_fbo.init(width, height);

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

	m_pingpongBuffer[0].init(width, height);
	m_pingpongBuffer[1].init(width, height);
}

void EffectRenderer::loadShaders() {
	m_shader.createProgram();
	m_shader.addShader(GL_VERTEX_SHADER, "resources/shaders/screen.v.glsl");
	m_shader.addShader(GL_FRAGMENT_SHADER, "resources/shaders/screen.f.glsl");
	m_shader.linkProgram();

	gk::Shader::bind(&m_shader);
	m_shader.setUniform("colorTexture", 0);
	m_shader.setUniform("bloomTexture", 1);
	m_shader.setUniform("depthTexture", 2);
	gk::Shader::bind(nullptr);

	m_blurShader.createProgram();
	m_blurShader.addShader(GL_VERTEX_SHADER, "resources/shaders/blur.v.glsl");
	m_blurShader.addShader(GL_FRAGMENT_SHADER, "resources/shaders/blur.f.glsl");
	m_blurShader.linkProgram();

	gk::Shader::bind(&m_blurShader);
	m_blurShader.setUniform("sampleTexture", 0);
	gk::Shader::bind(nullptr);
}

void EffectRenderer::begin() const {
	Framebuffer::bind(&m_fbo);

	static GLenum buffers[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
	glDrawBuffers(2, buffers);

	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
}

void EffectRenderer::end() const {
	Framebuffer::bind(nullptr);
	glClear(GL_COLOR_BUFFER_BIT);
}

void EffectRenderer::render() const {
	glDisable(GL_DEPTH_TEST);

	// Bloom effect
	if (Config::isBloomEffectEnabled) {
		bool horizontal = true;
		bool firstIteration = true;
		int amount = 10;
		gk::Shader::bind(&m_blurShader);
		for (int i = 0 ; i < amount ; ++i) {
			Framebuffer::bind(&m_pingpongBuffer[horizontal]);
			m_blurShader.setUniform("horizontal", (int)horizontal);

			if (firstIteration)
				m_fbo.bindBloomTexture(0);
			else
				m_pingpongBuffer[!horizontal].bindColorTexture(0);

			renderQuad();

			horizontal = !horizontal;
			if (firstIteration)
				firstIteration = false;
		}

		Framebuffer::bind(nullptr);

		m_pingpongBuffer[!horizontal].bindColorTexture(1);
	}
	else
		m_fbo.bindBloomTexture(1);

	m_fbo.bindColorTexture(0);
	m_fbo.bindDepthTexture(2);

	gk::Shader::bind(&m_shader);
	m_shader.setUniform("u_effectType", Config::currentScreenEffect);
	m_shader.setUniform("u_fogDepth", Config::fogDepth);
	m_shader.setUniform("u_fogColor", Config::fogColor);

	renderQuad();

	gk::Shader::bind(nullptr);

	glActiveTexture(GL_TEXTURE0);

	glEnable(GL_DEPTH_TEST);
}

void EffectRenderer::renderQuad() const {
	gk::VertexBuffer::bind(&m_vbo);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	m_vbo.setAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(0 * sizeof(float)));
	m_vbo.setAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	gk::VertexBuffer::bind(nullptr);
}

