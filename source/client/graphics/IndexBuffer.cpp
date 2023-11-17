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

#include "client/graphics/IndexBuffer.hpp"

IndexBuffer::IndexBuffer() {
}

IndexBuffer::IndexBuffer(IndexBuffer &&indexBuffer) {
	if (isValid()) free();

	m_staticHandle.idx = indexBuffer.m_staticHandle.idx;
	indexBuffer.m_staticHandle.idx = bgfx::kInvalidHandle;

	m_dynamicHandle.idx = indexBuffer.m_dynamicHandle.idx;
	indexBuffer.m_dynamicHandle.idx = bgfx::kInvalidHandle;

	m_isDynamic = indexBuffer.m_isDynamic;
}

IndexBuffer::~IndexBuffer() {
	free();
}

IndexBuffer &IndexBuffer::operator=(IndexBuffer &&indexBuffer) {
	if (isValid()) free();

	m_staticHandle.idx = indexBuffer.m_staticHandle.idx;
	indexBuffer.m_staticHandle.idx = bgfx::kInvalidHandle;

	m_dynamicHandle.idx = indexBuffer.m_dynamicHandle.idx;
	indexBuffer.m_dynamicHandle.idx = bgfx::kInvalidHandle;

	m_isDynamic = indexBuffer.m_isDynamic;

	return *this;
}

void IndexBuffer::init(const void *data, u32 size, bool isDynamic) {
	assert(size != 0);

	if (isDynamic || data == nullptr) {
		if (isValid())
			update(data, size);
		else {
			m_isDynamic = true;
			const bgfx::Memory *mem = (data ? bgfx::copy(data, size) : bgfx::alloc(size));
			m_dynamicHandle = bgfx::createDynamicIndexBuffer(mem);
			assert(bgfx::isValid(m_dynamicHandle));
		}
	}
	else {
		assert(!isValid());

		m_isDynamic = false;
		m_staticHandle = bgfx::createIndexBuffer(bgfx::copy(data, size));
		assert(bgfx::isValid(m_staticHandle));
	}
}

void IndexBuffer::update(const void *data, u32 size, u32 offset) const {
	assert(size != 0);
	assert(m_isDynamic);
	assert(bgfx::isValid(m_dynamicHandle));

	bgfx::update(m_dynamicHandle, offset, bgfx::copy(data, size));
}

void IndexBuffer::free() {
	if (!m_isDynamic && bgfx::isValid(m_staticHandle)) {
		bgfx::destroy(m_staticHandle);
		m_staticHandle.idx = bgfx::kInvalidHandle;
	}
	else if (m_isDynamic && bgfx::isValid(m_dynamicHandle)) {
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

void IndexBuffer::enable(u32 startIndex, u32 numIndices) const {
	if (!m_isDynamic) {
		assert(bgfx::isValid(m_staticHandle));
		bgfx::setIndexBuffer(m_staticHandle, startIndex, numIndices);
	}
	else {
		assert(bgfx::isValid(m_dynamicHandle));
		bgfx::setIndexBuffer(m_dynamicHandle, startIndex, numIndices);
	}
}

bool IndexBuffer::isValid() const {
	return bgfx::isValid(m_staticHandle) || bgfx::isValid(m_dynamicHandle);
}
