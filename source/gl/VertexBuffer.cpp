/*
 * =====================================================================================
 *
 *       Filename:  VertexBuffer.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  15/12/2014 17:10:16
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Quentin BAZIN, <quent42340@gmail.com>
 *        Company:  
 *
 * =====================================================================================
 */
#include "VertexBuffer.hpp"

VertexBuffer *VertexBuffer::activeBuffer = nullptr;

VertexBuffer::VertexBuffer() {
	glGenBuffers(1, &m_id);
}

VertexBuffer::~VertexBuffer() {
}

void VertexBuffer::setData(GLsizeiptr size, const GLvoid *data, GLenum usage) {
	if(activeBuffer != this) bind(this);
	
	glBufferData(GL_ARRAY_BUFFER, size, data, usage);
	
	if(activeBuffer != this) bind(nullptr);
}

void VertexBuffer::updateData(GLintptr offset, GLsizeiptr size, const GLvoid *data) {
	if(activeBuffer != this) bind(this);
	
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
	
	if(activeBuffer != this) bind(nullptr);
}

void VertexBuffer::bind(const VertexBuffer *vertexBuffer) {
	activeBuffer = const_cast<VertexBuffer*>(vertexBuffer);
	
	if(vertexBuffer) {
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->m_id);
	} else {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

