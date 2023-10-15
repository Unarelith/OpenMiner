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
#include <cassert>

#include <bx/bx.h>

#include "VertexBuffer.hpp"

VertexBuffer::VertexBuffer() {
}

VertexBuffer::VertexBuffer(VertexBuffer &&vertexBuffer) {
	m_staticHandle = vertexBuffer.m_staticHandle;
	vertexBuffer.m_staticHandle.idx = bgfx::kInvalidHandle;

	m_dynamicHandle = vertexBuffer.m_dynamicHandle;
	vertexBuffer.m_dynamicHandle.idx = bgfx::kInvalidHandle;

	m_layout = bx::move(m_layout);

	m_isDynamic = vertexBuffer.m_isDynamic;

	m_data = vertexBuffer.m_data;
	vertexBuffer.m_data = nullptr;
}

VertexBuffer::~VertexBuffer() {
	free();
}

VertexBuffer &VertexBuffer::operator=(VertexBuffer &&vertexBuffer) {
	m_staticHandle = vertexBuffer.m_staticHandle;
	vertexBuffer.m_staticHandle.idx = bgfx::kInvalidHandle;

	m_dynamicHandle = vertexBuffer.m_dynamicHandle;
	vertexBuffer.m_dynamicHandle.idx = bgfx::kInvalidHandle;

	m_layout = bx::move(m_layout);

	m_isDynamic = vertexBuffer.m_isDynamic;

	m_data = vertexBuffer.m_data;
	vertexBuffer.m_data = nullptr;

	return *this;
}

void VertexBuffer::setupDefaultLayout() {
	m_layout.begin()
	        .add(bgfx::Attrib::Position,  4, bgfx::AttribType::Float, false)
	        .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float, false)
	        .add(bgfx::Attrib::Color0,    4, bgfx::AttribType::Float, false)
	        .add(bgfx::Attrib::Normal,    3, bgfx::AttribType::Float, false)
	        .add(bgfx::Attrib::TexCoord1, 2, bgfx::AttribType::Float, false) // lightValue
	        .add(bgfx::Attrib::TexCoord2, 1, bgfx::AttribType::Float, false) // ambientOcclusion
	        .end();
}

void VertexBuffer::init(const void *data, uint32_t size, bool isDynamic) {
	if (isDynamic || data == nullptr) {
		if (isValid())
			update(data, size);
		else {
			m_isDynamic = true;
			m_data = (data ? bgfx::copy(data, size) : bgfx::alloc(size));
			m_dynamicHandle = bgfx::createDynamicVertexBuffer(m_data, m_layout);
		}
	}
	else {
		assert(!isValid());

		m_isDynamic = false;
		m_data = bgfx::copy(data, size);
		m_staticHandle = bgfx::createVertexBuffer(m_data, m_layout);
	}
}

void VertexBuffer::update(const void *data, uint32_t size, uint32_t offset) const {
	assert(m_isDynamic);
	assert(bgfx::isValid(m_dynamicHandle));

	bgfx::update(m_dynamicHandle, offset, bgfx::copy(data, size));
}

void VertexBuffer::free() {
	if (!m_isDynamic) {
		bgfx::destroy(m_staticHandle);
		m_staticHandle.idx = bgfx::kInvalidHandle;
	}
	else {
		bgfx::destroy(m_dynamicHandle);
		m_dynamicHandle.idx = bgfx::kInvalidHandle;
	}
}

void VertexBuffer::enable() const {
	if (!m_isDynamic) {
		assert(bgfx::isValid(m_staticHandle));
		bgfx::setVertexBuffer(0, m_staticHandle);
	}
	else {
		assert(bgfx::isValid(m_dynamicHandle));
		bgfx::setVertexBuffer(0, m_dynamicHandle);
	}
}

void VertexBuffer::enable(uint32_t startVertex, uint32_t numVertices) const {
	if (!m_isDynamic) {
		assert(bgfx::isValid(m_staticHandle));
		bgfx::setVertexBuffer(0, m_staticHandle, startVertex, numVertices);
	}
	else {
		assert(bgfx::isValid(m_dynamicHandle));
		bgfx::setVertexBuffer(0, m_dynamicHandle, startVertex, numVertices);
	}
}

bool VertexBuffer::isValid() {
	return bgfx::isValid(m_staticHandle) || bgfx::isValid(m_dynamicHandle);
}

