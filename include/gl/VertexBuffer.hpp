/*
 * =====================================================================================
 *
 *       Filename:  VertexBuffer.hpp
 *
 *    Description:
 *
 *        Created:  15/12/2014 17:09:58
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef VERTEXBUFFER_HPP_
#define VERTEXBUFFER_HPP_

#include "OpenGL.hpp"

class VertexBuffer {
	public:
		VertexBuffer(GLenum mode, GLint first, GLsizei count);
		~VertexBuffer();

		void setData(GLsizeiptr size, const GLvoid *data, GLenum usage) const;
		void updateData(GLintptr offset, GLsizeiptr size, const GLvoid *data) const;

		GLenum mode() const { return m_mode; }
		GLint first() const { return m_first; }
		GLsizei count() const { return m_count; }

		static void bind(const VertexBuffer *vertexBuffer);

	private:
		GLuint m_id;

		GLenum m_mode;
		GLint m_first;
		GLsizei m_count;
};

#endif // VERTEXBUFFER_HPP_
