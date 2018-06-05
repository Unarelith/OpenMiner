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

Shader::Shader(const std::string &vertexFilename, const std::string &fragmentFilename) {
	loadFromFile(vertexFilename, fragmentFilename);
}

Shader::~Shader() {
	while(m_vertexShaders.size() != 0) {
		glDeleteShader(m_vertexShaders.back());
		m_vertexShaders.pop_back();
	}

	while(m_fragmentShaders.size() != 0) {
		glDeleteShader(m_fragmentShaders.back());
		m_fragmentShaders.pop_back();
	}

	glDeleteProgram(m_program);
}

void Shader::loadFromFile(const std::string &vertexFilename, const std::string &fragmentFilename) {
	createProgram();

	addShader(GL_VERTEX_SHADER, vertexFilename);
	addShader(GL_FRAGMENT_SHADER, fragmentFilename);

	linkProgram();
}

void Shader::createProgram() {
	m_program = glCreateProgram();
}

void Shader::linkProgram() {
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

void Shader::addShader(GLenum type, const std::string &filename) {
	GLuint shader = glCreateShader(type);

	if(type == GL_VERTEX_SHADER) {
		m_vertexShaders.push_back(shader);
	} else {
		m_fragmentShaders.push_back(shader);
	}

	std::ifstream file(filename);
	if(!file) {
		glDeleteShader(shader);

		throw EXCEPTION("Failed to open", filename);
	}

	std::string line;
	std::string sourceCode;

	while(getline(file, line)) sourceCode += line + '\n';
	file.close();

	const GLchar *sourceCodeString = sourceCode.c_str();

	glShaderSource(shader, 1, &sourceCodeString, nullptr);

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

	glAttachShader(m_program, shader);
}

GLint Shader::attrib(const std::string &name) {
	GLint attrib = glGetAttribLocation(m_program, name.c_str());

	if(attrib == -1) {
		DEBUG("Could not bind attribute:", name);
	}

	return attrib;
}

GLint Shader::uniform(const std::string &name) {
	GLint uniform = glGetUniformLocation(m_program, name.c_str());

	if(uniform == -1) {
		DEBUG("Could not bind uniform:", name);
	}

	return uniform;
}

void Shader::enableVertexAttribArray(const std::string &name) {
	glEnableVertexAttribArray(attrib(name));
}

void Shader::disableVertexAttribArray(const std::string &name) {
	glDisableVertexAttribArray(attrib(name));
}

void Shader::setUniform(const std::string &name, int n) {
	glUniform1i(uniform(name), n);
}

void Shader::setUniform(const std::string &name, const glm::mat4 &matrix) {
	glUniformMatrix4fv(uniform(name), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::bind(const Shader *shader) {
	if(shader) {
		glUseProgram(shader->m_program);
	} else {
		glUseProgram(0);
	}
}

