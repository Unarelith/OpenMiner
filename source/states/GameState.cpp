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

#include "GameState.hpp"

GameState::GameState() {
	m_shader.loadFromFile("shaders/game.v.glsl", "shaders/game.f.glsl");
	
	Shader::bind(&m_shader);
	
	m_projectionMatrix = glm::perspective(45.0f, 640.0f / 480.0f, 0.1f, 3000.0f);
	/*glm::mat4 modelviewMatrix = glm::lookAt(glm::vec3(-20.0f, 40.0f, -20.0f),
											glm::vec3(16.0f, 16.0f, 16.0f),
											glm::vec3(0.0f, 1.0f, 0.0f));
	*/
	m_viewMatrix = m_camera.update();
	
	//glActiveTexture(GL_TEXTURE0);
	
	glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
	
	m_shader.setUniform("u_tex", 0);
	
	Shader::bind(nullptr);
}

GameState::~GameState() {
}

void GameState::update() {
	m_viewMatrix = m_camera.processInputs();
	
	//Shader::bind(&m_shader);
	//m_shader.setUniform("u_MVP", m_projectionMatrix * m_modelviewMatrix);
	//Shader::bind(nullptr);
}

void GameState::draw() {
	Shader::bind(&m_shader);
	
	m_world.draw(m_shader, m_projectionMatrix * m_viewMatrix);
	
	/*glBegin(GL_LINES);
		glColor3f(1, 1, 1); glVertex3f(0, 0, 0);
		glColor3f(1, 1, 1); glVertex3f(0, 5, 0);
		
		glColor3f(1, 0, 1); glVertex3f(0, 0, 0);
		glColor3f(1, 0, 1); glVertex3f(5, 0, 0);
		
		glColor3f(1, 1, 0); glVertex3f(0, 0, 0);
		glColor3f(1, 1, 0); glVertex3f(0, 0, 5);
	glEnd();*/
	
	Shader::bind(nullptr);
}

