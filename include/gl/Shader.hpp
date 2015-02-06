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

#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "OpenGL.hpp"

class Shader {
	public:
		Shader();
		Shader(const char *vertexFilename, const char *fragementFilename);
		~Shader();
		
		void loadFromFile(const char *vertexFilename, const char *fragementFilename);
		
		void createProgram();
		void linkProgram();
		
		void addShader(GLenum type, const char *filename);
		
		GLint attrib(std::string name);
		GLint uniform(std::string name);
		
		void enableVertexAttribArray(std::string name);
		void disableVertexAttribArray(std::string name);
		
		void setUniform(std::string name, int n);
		void setUniform(std::string name, const glm::mat4 &matrix);
		
		static void bind(const Shader *shader);
		
		GLint program() const { return m_program; }
		
	private:
		std::vector<GLuint> m_vertexShaders;
		std::vector<GLuint> m_fragmentShaders;
		GLuint m_program;
};

#endif // SHADER_HPP_
