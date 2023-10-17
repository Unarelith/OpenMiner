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

	initDefaultIndexBuffer();
	initCubeIndexBuffer();
}

void RenderTarget::free() {
	m_cubeIndexBuffer.free();
	m_defaultIndexBuffer.free();

	bgfx::destroy(m_samplerUniform);
}

void RenderTarget::draw(const Drawable &drawable, const RenderStates &states) {
	drawable.draw(*this, states);
}

void RenderTarget::draw(const VertexBuffer &vertexBuffer, u32 firstVertex, u32 vertexCount, const RenderStates &states) {
	beginDrawing(states);

	if (firstVertex == 0 && vertexCount == 0)
		vertexBuffer.enable();
	else
		vertexBuffer.enable(firstVertex, vertexCount);

	bgfx::setTransform(states.transform.getRawMatrix());

	bgfx::submit(states.view, states.shader->program());
}

void RenderTarget::drawElements(const VertexBuffer &vertexBuffer, const IndexBuffer &indexBuffer, u32 firstVertex, u32 vertexCount, const RenderStates &states) {
	beginDrawing(states);

	vertexBuffer.enable();

	if (firstVertex == 0 && vertexCount == 0)
		indexBuffer.enable();
	else
		indexBuffer.enable(firstVertex, vertexCount);

	bgfx::setTransform(states.transform.getRawMatrix());

	bgfx::submit(states.view, states.shader->program());
}

void RenderTarget::beginDrawing(const RenderStates &states) {
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

	if (states.texture)
		states.texture->enable(0, m_samplerUniform);

	setBgfxState(states);
}

void RenderTarget::setBgfxState(const RenderStates &states) {
	u64 state = BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z | BGFX_STATE_MSAA;

	if (states.isDepthTestEnabled)
		state |= BGFX_STATE_DEPTH_TEST_LESS;

	if (states.isCullFaceEnabled)
		state |= BGFX_STATE_CULL_CW;

	if (states.isBlendingEnabled) {
		state |= BGFX_STATE_BLEND_FUNC_SEPARATE(
			states.blendFuncColorSrc, states.blendFuncColorDst,
			states.blendFuncAlphaSrc, states.blendFuncAlphaDst
		);

		state |= BGFX_STATE_BLEND_EQUATION(states.blendEquation);
	}

	state |= states.primitiveType;

	bgfx::setState(state);
}

gk::IntRect RenderTarget::getViewport(const View& view) const {
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

void RenderTarget::initDefaultIndexBuffer() {
	u16 indices[6 * 5]{
		0, 1, 3,
		3, 1, 2
	};

	for (u8 i = 1 ; i < 5 ; ++i) {
		for (u8 j = 0 ; j < 6 ; ++j) {
			indices[i * 6 + j] = u16(indices[j] + 4 * i);
		}
	}

	m_defaultIndexBuffer.init(indices, 6 * 5 * sizeof(u16));
}

void RenderTarget::initCubeIndexBuffer() {
	const u16 indices[6 * 6] = {
		0, 1, 2,
		2, 3, 0,

		4, 5, 6,
		6, 7, 4,

		8, 9, 10,
		10, 11, 8,

		12, 13, 14,
		14, 15, 12,

		16, 17, 18,
		18, 19, 16,

		20, 21, 22,
		22, 23, 20,
	};

	m_cubeIndexBuffer.init(indices, sizeof(indices));
}
