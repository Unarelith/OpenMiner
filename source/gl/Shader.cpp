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

GLint Shader::attrib(const char *attribName) {
	GLint attrib = glGetAttribLocation(m_program, attribName);
	
	if(attrib == -1) {
		throw EXCEPTION("Could not bind attribute:", attribName);
	}
	
	return attrib;
}

GLint Shader::uniform(const char *uniformName) {
	GLint uniform = glGetUniformLocation(m_program, uniformName);
	
	if(uniform == -1) {
		throw EXCEPTION("Could not bind uniform:", uniformName);
	}
	
	return uniform;
}

void Shader::enableVertexAttribArray(const char *attribName) {
	glEnableVertexAttribArray(attrib(attribName));
}

void Shader::disableVertexAttribArray(const char *attribName) {
	glDisableVertexAttribArray(attrib(attribName));
}

void Shader::bind(const Shader *shader) {
	if(shader) {
		glUseProgram(shader->m_program);
	} else {
		glUseProgram(0);
	}
}

