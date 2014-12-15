/*
 * =====================================================================================
 *
 *       Filename:  GameState.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  15/12/2014 03:51:55
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Quentin BAZIN, <quent42340@gmail.com>
 *        Company:  
 *
 * =====================================================================================
 */
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GameState.hpp"

GameState::GameState() {
	m_shader.loadFromFile("shaders/game.v.glsl", "shaders/game.f.glsl");
	
	glGenBuffers(1, &m_vbo);
	
	GLfloat vertices[2 * 4] = {
		0.0, 1.0,
		1.0, 0.0,
		0.0, -1.0,
		-1.0, 0.0
	};
	
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	Shader::bind(&m_shader);
	
	glm::mat4 projectionMatrix = glm::perspective(80.0f, 1.0f, 0.1f, 1000.0f);
	glUniformMatrix4fv(m_shader.uniform("u_projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	
	glm::mat4 viewMatrix = glm::lookAt(glm::vec3(0.0f, 1.0f, 1.0f),
									   glm::vec3(0.0f, 0.0f, 0.0f),
									   glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(m_shader.uniform("u_viewMatrix"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
	
	Shader::bind(nullptr);
}

GameState::~GameState() {
	glDeleteBuffers(1, &m_vbo);
}

void GameState::update() {
}

void GameState::draw() {
	Shader::bind(&m_shader);
	
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	
	glVertexAttribPointer(m_shader.attrib("coord2d"), 2, GL_FLOAT, GL_FALSE, 0, 0);
	
	m_shader.enableVertexAttribArray("coord2d");
	
	GLubyte indices[6] = {
		0, 1, 2,
		2, 3, 0
	};
	
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);
	
	m_shader.disableVertexAttribArray("coord2d");
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	Shader::bind(nullptr);
}

