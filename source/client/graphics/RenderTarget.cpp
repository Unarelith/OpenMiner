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
#include <gk/gl/Texture.hpp>

#include "Drawable.hpp"
#include "IndexBuffer.hpp"
#include "RenderTarget.hpp"
#include "Shader.hpp"
#include "VertexBuffer.hpp"

const RenderStates RenderStates::Default{};

void RenderTarget::draw(const Drawable &drawable, const RenderStates &states) {
	drawable.draw(*this, states);
}

void RenderTarget::draw(const VertexBuffer &vertexBuffer, uint64_t mode, uint32_t firstVertex, uint32_t vertexCount, const RenderStates &states) {
#ifdef OM_NOT_IMPLEMENTED
	beginDrawing(states);
#endif // OM_NOT_IMPLEMENTED

	vertexBuffer.enable(firstVertex, vertexCount);

	bgfx::setTransform(states.transform.getRawMatrix());

	bgfx::submit(0, states.shader->program());
}

void RenderTarget::drawElements(const VertexBuffer &vertexBuffer, const IndexBuffer &indexBuffer, uint64_t mode, uint32_t count, const RenderStates &states) {
#ifdef OM_NOT_IMPLEMENTED
	beginDrawing(states);
#endif // OM_NOT_IMPLEMENTED

	if (count == 0)
		vertexBuffer.enable();
	else
		vertexBuffer.enable(0, count);

	indexBuffer.enable();

	bgfx::setTransform(states.transform.getRawMatrix());

	bgfx::submit(0, states.shader->program());
}

void RenderTarget::beginDrawing(const RenderStates &states) {
	//----------------------------------------------------------------------------
	// Shader & uniforms
	//----------------------------------------------------------------------------
	if (!states.shader) return;

	static const Shader *previousShader = nullptr;

	if (!m_view) {
		bgfx::setViewTransform(0,
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
		bgfx::setViewRect(0, (uint16_t)viewport.x, (uint16_t)top, (uint16_t)viewport.sizeX, (uint16_t)viewport.sizeY);
		m_previousViewport = viewport;
	}

	bgfx::setViewTransform(0,
		m_view->getViewTransform().getRawMatrix(),
		m_view->getTransform().getRawMatrix());

	m_viewChanged = false;
}
