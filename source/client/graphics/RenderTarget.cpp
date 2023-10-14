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
#include <gk/gl/GLCheck.hpp>
#include <gk/gl/Shader.hpp>
#include <gk/gl/Texture.hpp>

#include "Drawable.hpp"
#include "RenderTarget.hpp"
#include "VertexBuffer.hpp"

const RenderStates RenderStates::Default{};

void RenderTarget::draw(const Drawable &drawable, const RenderStates &states) {
	drawable.draw(*this, states);
}

void RenderTarget::draw(const VertexBuffer &vertexBuffer, GLenum mode, GLint firstVertex, GLsizei vertexCount, const RenderStates &states) {
	beginDrawing(states);

	VertexBuffer::bind(&vertexBuffer);

	vertexBuffer.layout().enableLayout();
	states.shader->setUniform("u_modelMatrix", states.transform);

	glCheck(::glDrawArrays(mode, firstVertex, vertexCount));

	vertexBuffer.layout().disableLayout();

	VertexBuffer::bind(nullptr);
}

void RenderTarget::drawElements(const VertexBuffer &vertexBuffer, GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, const RenderStates &states) {
	beginDrawing(states);

	VertexBuffer::bind(&vertexBuffer);

	vertexBuffer.layout().enableLayout();
	states.shader->setUniform("u_modelMatrix", states.transform);

	glCheck(glDrawElements(mode, count, type, indices));

	vertexBuffer.layout().disableLayout();

	VertexBuffer::bind(nullptr);
}

void RenderTarget::drawArrays(GLenum mode, GLint firstVertex, GLsizei vertexCount) {
	glCheck(::glDrawArrays(mode, firstVertex, vertexCount));
}

void RenderTarget::beginDrawing(const RenderStates &states) {
	//----------------------------------------------------------------------------
	// Shader & uniforms
	//----------------------------------------------------------------------------
	if (!states.shader) return;

	static const gk::Shader *previousShader = nullptr;

	gk::Shader::bind(states.shader);

	if (!m_view) {
		states.shader->setUniform("u_projectionMatrix", states.projectionMatrix);
		states.shader->setUniform("u_viewMatrix", states.viewMatrix);
	}
	else if (m_viewChanged || states.shader != previousShader)
		applyCurrentView(states);

	previousShader = states.shader;

	//----------------------------------------------------------------------------
	// Texture
	//----------------------------------------------------------------------------
	if (states.texture)
		gk::Texture::bind(states.texture);
}

gk::IntRect RenderTarget::getViewport(const gk::View& view) const {
	float width  = static_cast<float>(getSize().x);
	float height = static_cast<float>(getSize().y);
	const gk::FloatRect& viewport = view.getViewport();

	return gk::IntRect(static_cast<int>(0.5f + width  * viewport.x),
	                   static_cast<int>(0.5f + height * viewport.y),
	                   static_cast<int>(width  * viewport.sizeX),
	                   static_cast<int>(height * viewport.sizeY));
}

void RenderTarget::applyCurrentView(const RenderStates &states) {
	gk::IntRect viewport = getViewport(*m_view);
	if (viewport != m_previousViewport) {
		int top = getSize().y - (viewport.y + viewport.sizeY);
		glViewport(viewport.x, top, viewport.sizeX, viewport.sizeY);
		m_previousViewport = viewport;
	}

	states.shader->setUniform("u_projectionMatrix", m_view->getTransform());
	states.shader->setUniform("u_viewMatrix", m_view->getViewTransform());

	m_viewChanged = false;
}
