/*
 * =====================================================================================
 *
 *       Filename:  GameState.cpp
 *
 *    Description:
 *
 *        Created:  15/12/2014 03:51:55
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#include "OpenGL.hpp"

#include "Config.hpp"
#include "Mouse.hpp"
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

	// m_projectionMatrix = glm::perspective(45.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, DIST_NEAR, DIST_FAR);
	m_projectionMatrix = glm::perspective(45.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, DIST_NEAR, DIST_FAR);
	m_viewMatrix = m_camera.update();

	m_shader.setUniform("u_projectionMatrix", m_projectionMatrix);

	m_shader.setUniform("u_tex", 0);

	Shader::bind(nullptr);
}

void GameState::update() {
	m_viewMatrix = m_camera.processInputs();

	// glm::vec3 start = glm::unProject(glm::vec3(Mouse::getX(), Mouse::getY(), 0.0), m_viewMatrix, m_projectionMatrix, glm::ivec4(0.0, 0.0, SCREEN_WIDTH, SCREEN_HEIGHT));
	// glm::vec3 end = glm::unProject(glm::vec3(Mouse::getX(), Mouse::getY(), 1.0), m_viewMatrix, m_projectionMatrix, glm::ivec4(0.0, 0.0, SCREEN_WIDTH, SCREEN_HEIGHT));
	m_world.testCubes(m_camera);

	// m_world.removeSelectedBlock();
}

void GameState::draw() {
	Shader::bind(&m_shader);

	m_shader.setUniform("u_viewMatrix", m_viewMatrix);

	// m_skybox.draw(m_shader);

	m_world.draw(m_shader, m_projectionMatrix, m_viewMatrix);

	Shader::bind(nullptr);
}

