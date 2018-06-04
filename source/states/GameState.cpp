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

GameState::GameState() : m_camera(Camera::getInstance()) {
	m_shader.createProgram();

	m_shader.addShader(GL_VERTEX_SHADER, "shaders/game.v.glsl");
	m_shader.addShader(GL_FRAGMENT_SHADER, "shaders/color.f.glsl");
	m_shader.addShader(GL_FRAGMENT_SHADER, "shaders/light.f.glsl");
	m_shader.addShader(GL_FRAGMENT_SHADER, "shaders/fog.f.glsl");
	m_shader.addShader(GL_FRAGMENT_SHADER, "shaders/game.f.glsl");

	m_shader.linkProgram();

	Shader::bind(&m_shader);

	m_projectionMatrix = glm::perspective(45.0f, 640.0f / 480.0f, 0.1f, 1000.0f);
	m_viewMatrix = m_camera.update();

	m_shader.setUniform("u_projectionMatrix", m_projectionMatrix);

	m_shader.setUniform("u_tex", 0);

	Shader::bind(nullptr);
}

GameState::~GameState() {
}

void GameState::update() {
	m_viewMatrix = m_camera.processInputs();
}

void GameState::draw() {
	Shader::bind(&m_shader);

	m_shader.setUniform("u_viewMatrix", m_viewMatrix);

	//m_skybox.draw(m_shader);

	m_world.draw(m_shader, m_projectionMatrix, m_viewMatrix);

	Shader::bind(nullptr);
}

