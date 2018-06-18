/*
 * =====================================================================================
 *
 *       Filename:  VertexBuffer.cpp
 *
 *    Description:
 *
 *        Created:  15/12/2014 17:10:16
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "VertexBuffer.hpp"

VertexBuffer::VertexBuffer(GLenum mode, GLint first, GLsizei count) {
	glGenBuffers(1, &m_id);

	m_mode = mode;
	m_first = first;
	m_count = count;
}

VertexBuffer::~VertexBuffer() {
	glDeleteBuffers(1, &m_id);
}

void VertexBuffer::setData(GLsizeiptr size, const GLvoid *data, GLenum usage) const {
	glBufferData(GL_ARRAY_BUFFER, size, data, usage);
}

void VertexBuffer::updateData(GLintptr offset, GLsizeiptr size, const GLvoid *data) const {
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}

void VertexBuffer::setAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer) const {
	glVertexAttribPointer(index, size, type, normalized, stride, pointer);
}

void VertexBuffer::bind(const VertexBuffer *vertexBuffer) {
	if(vertexBuffer) {
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->m_id);
	} else {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

