/*
 * =====================================================================================
 *
 *       Filename:  Shader.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  15/12/2014 16:30:34
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Quentin BAZIN, <quent42340@gmail.com>
 *        Company:  
 *
 * =====================================================================================
 */
#include <iostream>
#include <fstream>

#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>

#include "Exception.hpp"
#include "Shader.hpp"

Shader::Shader() {
}

Shader::Shader(const char *vertexFilename, const char *fragmentFilename) {
	loadFromFile(vertexFilename, fragmentFilename);
}

Shader::~Shader() {
	glDeleteShader(m_vertexShader);
	glDeleteShader(m_fragmentShader);
	glDeleteProgram(m_program);
}

void Shader::loadFromFile(const char *vertexFilename, const char *fragmentFilename) {
	compileShader(GL_VERTEX_SHADER, m_vertexShader, vertexFilename);
	compileShader(GL_FRAGMENT_SHADER, m_fragmentShader, fragmentFilename);
	
	m_program = glCreateProgram();
	
	glAttachShader(m_program, m_vertexShader);
	glAttachShader(m_program, m_fragmentShader);
	
	glLinkProgram(m_program);
	
	GLint linkOK = GL_FALSE;
	glGetProgramiv(m_program, GL_LINK_STATUS, &linkOK);
	if(!linkOK){
		GLint errorSize = 0;
		glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &errorSize);
		
		char *errorMsg = new char[errorSize + 1];
		
		glGetProgramInfoLog(m_program, errorSize, &errorSize, errorMsg);
		errorMsg[errorSize] = '\0';
		
		std::string error = std::string(errorMsg);
		
		delete[] errorMsg;
		glDeleteProgram(m_program);
		
		throw EXCEPTION("Program", m_program, "link failed:", error);
	}
}

void Shader::compileShader(GLenum type, GLuint &shader, const char *filename) {
	shader = glCreateShader(type);
	
	std::ifstream file(filename);
	if(!file) {
		glDeleteShader(type);
		
		throw EXCEPTION("Failed to open", filename);
	}
	
	std::string line;
	std::string sourceCode;
	
	while(getline(file, line)) sourceCode += line + '\n';
	file.close();
	
	const GLchar *sourceCodeString = sourceCode.c_str();
	
	glShaderSource(shader, 1, &sourceCodeString, NULL);
	
	glCompileShader(shader);
	
	GLint compileOK = GL_FALSE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileOK);
	if(!compileOK) {
		GLint errorSize = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &errorSize);
		
		char *errorMsg = new char[errorSize + 1];
		
		glGetShaderInfoLog(shader, errorSize, &errorSize, errorMsg);
		errorMsg[errorSize] = '\0';
		
		std::string error = std::string(errorMsg);
		
		delete[] errorMsg;
		glDeleteShader(shader);
		
		throw EXCEPTION("Shader", filename, "compilation failed:", error);
	}
}

GLint Shader::attrib(std::string name) {
	GLint attrib = glGetAttribLocation(m_program, name.c_str());
	
	if(attrib == -1) {
		throw EXCEPTION("Could not bind attribute:", name);
	}
	
	return attrib;
}

GLint Shader::uniform(std::string name) {
	GLint uniform = glGetUniformLocation(m_program, name.c_str());
	
	if(uniform == -1) {
		throw EXCEPTION("Could not bind uniform:", name);
	}
	
	return uniform;
}

void Shader::enableVertexAttribArray(std::string name) {
	glEnableVertexAttribArray(attrib(name));
}

void Shader::disableVertexAttribArray(std::string name) {
	glDisableVertexAttribArray(attrib(name));
}

void Shader::setUniform(std::string name, int n) {
	glUniform1i(uniform(name), n);
}

void Shader::setUniform(std::string name, const glm::mat4 &matrix) {
	glUniformMatrix4fv(uniform(name), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::bind(const Shader *shader) {
	if(shader) {
		glUseProgram(shader->m_program);
	} else {
		glUseProgram(0);
	}
}

