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

#include "IndexBuffer.hpp"

IndexBuffer::IndexBuffer() {
}

IndexBuffer::IndexBuffer(IndexBuffer &&vertexBuffer) {
	m_staticHandle = vertexBuffer.m_staticHandle;
	vertexBuffer.m_staticHandle.idx = bgfx::kInvalidHandle;

	m_dynamicHandle = vertexBuffer.m_dynamicHandle;
	vertexBuffer.m_dynamicHandle.idx = bgfx::kInvalidHandle;

	m_isDynamic = vertexBuffer.m_isDynamic;

	m_data = vertexBuffer.m_data;
	vertexBuffer.m_data = nullptr;
}

IndexBuffer::~IndexBuffer() {
	free();
}

IndexBuffer &IndexBuffer::operator=(IndexBuffer &&vertexBuffer) {
	m_staticHandle = vertexBuffer.m_staticHandle;
	vertexBuffer.m_staticHandle.idx = bgfx::kInvalidHandle;

	m_dynamicHandle = vertexBuffer.m_dynamicHandle;
	vertexBuffer.m_dynamicHandle.idx = bgfx::kInvalidHandle;

	m_data = vertexBuffer.m_data;
	vertexBuffer.m_data = nullptr;

	return *this;
}

void IndexBuffer::init(const void *data, uint32_t size, bool isDynamic) {
	assert(!isValid());

	m_isDynamic = isDynamic || (data == nullptr);

	if (m_isDynamic) {
		m_data = (data ? bgfx::copy(data, size) :  bgfx::alloc(size));
		m_dynamicHandle = bgfx::createDynamicIndexBuffer(m_data);
	}
	else {
		m_data = bgfx::copy(data, size);
		m_staticHandle = bgfx::createIndexBuffer(m_data);
	}
}

void IndexBuffer::update(const void *data, uint32_t size, uint32_t offset) const {
	assert(m_isDynamic);
	assert(bgfx::isValid(m_dynamicHandle));

	bgfx::update(m_dynamicHandle, offset, bgfx::copy(data, size));
}

void IndexBuffer::free() {
	if (!m_isDynamic) {
		bgfx::destroy(m_staticHandle);
		m_staticHandle.idx = bgfx::kInvalidHandle;
	}
	else {
		bgfx::destroy(m_dynamicHandle);
		m_dynamicHandle.idx = bgfx::kInvalidHandle;
	}
}

void IndexBuffer::enable() const {
	if (!m_isDynamic) {
		assert(bgfx::isValid(m_staticHandle));
		bgfx::setIndexBuffer(m_staticHandle);
	}
	else {
		assert(bgfx::isValid(m_dynamicHandle));
		bgfx::setIndexBuffer(m_dynamicHandle);
	}
}

void IndexBuffer::enable(uint32_t startIndex, uint32_t numVertices) const {
	if (!m_isDynamic) {
		assert(bgfx::isValid(m_staticHandle));
		bgfx::setIndexBuffer(m_staticHandle, startIndex, numVertices);
	}
	else {
		assert(bgfx::isValid(m_dynamicHandle));
		bgfx::setIndexBuffer(m_dynamicHandle, startIndex, numVertices);
	}
}

bool IndexBuffer::isValid() {
	return bgfx::isValid(m_staticHandle) || bgfx::isValid(m_dynamicHandle);
}

