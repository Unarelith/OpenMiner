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
		Shader(const std::string &vertexFilename, const std::string &fragementFilename);
		~Shader();
		
		void loadFromFile(const std::string &vertexFilename, const std::string &fragementFilename);
		
		void createProgram();
		void linkProgram();
		
		void addShader(GLenum type, const std::string &filename);
		
		GLint attrib(const std::string &name);
		GLint uniform(const std::string &name);
		
		void enableVertexAttribArray(const std::string &name);
		void disableVertexAttribArray(const std::string &name);
		
		void setUniform(const std::string &name, int n);
		void setUniform(const std::string &name, const glm::mat4 &matrix);
		
		static void bind(const Shader *shader);
		
		GLint program() const { return m_program; }
		
	private:
		std::vector<GLuint> m_vertexShaders;
		std::vector<GLuint> m_fragmentShaders;
		
		GLuint m_program;
};

#endif // SHADER_HPP_
