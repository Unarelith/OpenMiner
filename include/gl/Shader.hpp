/*
 * =====================================================================================
 *
 *       Filename:  Shader.hpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  15/12/2014 16:30:20
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Quentin BAZIN, <quent42340@gmail.com>
 *        Company:  
 *
 * =====================================================================================
 */
#ifndef SHADER_HPP_
#define SHADER_HPP_

#include "OpenGL.hpp"

class Shader {
	public:
		Shader();
		Shader(const char *vertexFilename, const char *fragementFilename);
		~Shader();
		
		void loadFromFile(const char *vertexFilename, const char *fragementFilename);
		
		void compileShader(GLenum type, GLuint &shader, const char *filename);
		
		GLint attrib(const char *attribName);
		GLint uniform(const char *uniformName);
		
		void enableVertexAttribArray(const char *attribName);
		void disableVertexAttribArray(const char *attribName);
		
		static void bind(const Shader *shader);
		
		GLint program() const { return m_program; }
		
	private:
		GLuint m_vertexShader;
		GLuint m_fragmentShader;
		GLuint m_program;
};

#endif // SHADER_HPP_
