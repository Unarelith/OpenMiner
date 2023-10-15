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
#include "Drawable.hpp"
#include "IndexBuffer.hpp"
#include "RenderTarget.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "VertexBuffer.hpp"

const RenderStates RenderStates::Default{};

void RenderTarget::init() {
	m_samplerUniform = bgfx::createUniform("u_tex", bgfx::UniformType::Sampler);
}

void RenderTarget::free() {
	bgfx::destroy(m_samplerUniform);
}

void RenderTarget::draw(const Drawable &drawable, const RenderStates &states) {
	drawable.draw(*this, states);
}

void RenderTarget::draw(const VertexBuffer &vertexBuffer, uint64_t mode, uint32_t firstVertex, uint32_t vertexCount, const RenderStates &states) {
	beginDrawing(states);

	if (firstVertex == 0 && vertexCount == 0)
		vertexBuffer.enable();
	else
		vertexBuffer.enable(firstVertex, vertexCount);

	bgfx::setTransform(states.transform.getRawMatrix());

	bgfx::setState(
	     BGFX_STATE_WRITE_RGB
	   | BGFX_STATE_WRITE_A
	   | BGFX_STATE_WRITE_Z
	   // | BGFX_STATE_DEPTH_TEST_LESS
	   | BGFX_STATE_CULL_CW
	   | BGFX_STATE_MSAA
	);

	bgfx::submit(states.view, states.shader->program());
}

void RenderTarget::drawElements(const VertexBuffer &vertexBuffer, const IndexBuffer &indexBuffer, uint64_t mode, uint32_t count, const RenderStates &states) {
	beginDrawing(states);

	vertexBuffer.enable();

	if (count == 0)
		indexBuffer.enable();
	else
		indexBuffer.enable(0, count);

	bgfx::setTransform(states.transform.getRawMatrix());

	bgfx::setState(
	     BGFX_STATE_WRITE_RGB
	   | BGFX_STATE_WRITE_A
	   | BGFX_STATE_WRITE_Z
	   // | BGFX_STATE_DEPTH_TEST_LESS
	   | BGFX_STATE_CULL_CW
	   | BGFX_STATE_MSAA
	);

	bgfx::submit(states.view, states.shader->program());
}

void RenderTarget::beginDrawing(const RenderStates &states) {
	//----------------------------------------------------------------------------
	// Shader & uniforms
	//----------------------------------------------------------------------------
	if (!states.shader) return;

	static const Shader *previousShader = nullptr;

	if (!m_view) {
		bgfx::setViewTransform(states.view,
			states.viewMatrix.getRawMatrix(),
			states.projectionMatrix.getRawMatrix());
	}
	else if (m_viewChanged || states.shader != previousShader)
		applyCurrentView(states);

	previousShader = states.shader;

	//----------------------------------------------------------------------------
	// Texture
	//----------------------------------------------------------------------------
	if (states.texture)
		states.texture->enable(0, m_samplerUniform);
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
	int top = getSize().y - (viewport.y + viewport.sizeY);
	bgfx::setViewRect(states.view, (uint16_t)viewport.x, (uint16_t)top, (uint16_t)viewport.sizeX, (uint16_t)viewport.sizeY);

	bgfx::setViewTransform(states.view,
		m_view->getViewTransform().getRawMatrix(),
		m_view->getTransform().getRawMatrix());

	m_viewChanged = false;
}
