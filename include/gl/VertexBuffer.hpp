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
		VertexBuffer();
		~VertexBuffer();

		void setData(GLsizeiptr size, const GLvoid *data, GLenum usage);
		void updateData(GLintptr offset, GLsizeiptr size, const GLvoid *data);

		static void bind(const VertexBuffer *vertexBuffer);

	private:
		GLuint m_id;
};

#endif // VERTEXBUFFER_HPP_
