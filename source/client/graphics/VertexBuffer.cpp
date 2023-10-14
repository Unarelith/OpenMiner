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

#include "VertexBuffer.hpp"

VertexBuffer::VertexBuffer() {
	glCheck(glGenBuffers(1, &m_id));
}

VertexBuffer::VertexBuffer(VertexBuffer &&vertexBuffer) {
	m_id = vertexBuffer.m_id;
	vertexBuffer.m_id = 0;

	m_layout = std::move(vertexBuffer.m_layout);
}

VertexBuffer::~VertexBuffer() noexcept {
	if (m_id != 0)
		glCheck(glDeleteBuffers(1, &m_id));
}

VertexBuffer &VertexBuffer::operator=(VertexBuffer &&vertexBuffer) {
	m_id = vertexBuffer.m_id;
	vertexBuffer.m_id = 0;

	m_layout = std::move(vertexBuffer.m_layout);

	return *this;
}

void VertexBuffer::setData(GLsizeiptr size, const GLvoid *data, GLenum usage) const {
	glCheck(glBufferData(GL_ARRAY_BUFFER, size, data, usage));
}

void VertexBuffer::updateData(GLintptr offset, GLsizeiptr size, const GLvoid *data) const {
	glCheck(glBufferSubData(GL_ARRAY_BUFFER, offset, size, data));
}

void VertexBuffer::bind(const VertexBuffer *vertexBuffer) {
	if(vertexBuffer) {
		glCheck(glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->m_id));
	} else {
		glCheck(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}
}

