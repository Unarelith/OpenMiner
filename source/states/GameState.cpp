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
#include "GameClock.hpp"
#include "Keyboard.hpp"
#include "Mouse.hpp"
#include "GameState.hpp"

GameState::GameState() {
	initShaders();
}

void GameState::initShaders() {
	m_shader.createProgram();

	m_shader.addShader(GL_VERTEX_SHADER, "shaders/game.v.glsl");
	m_shader.addShader(GL_FRAGMENT_SHADER, "shaders/color.f.glsl");
	m_shader.addShader(GL_FRAGMENT_SHADER, "shaders/light.f.glsl");
	m_shader.addShader(GL_FRAGMENT_SHADER, "shaders/fog.f.glsl");
	m_shader.addShader(GL_FRAGMENT_SHADER, "shaders/game.f.glsl");

	m_shader.linkProgram();

	Shader::bind(&m_shader);

	m_projectionMatrix = glm::perspective(45.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, DIST_NEAR, DIST_FAR);
	m_shader.setUniform("u_projectionMatrix", m_projectionMatrix);
	m_shader.setUniform("u_tex", 0);
	m_shader.setUniform("u_time", 0);

	Shader::bind(nullptr);
}

void GameState::onEvent(const SDL_Event &event) {
	if (event.type == SDL_MOUSEMOTION) {
		if(SCREEN_WIDTH / 2 != event.motion.x || SCREEN_HEIGHT / 2 != event.motion.y) {
			m_camera.turnH(event.motion.xrel * 0.06);
			m_camera.turnV(-event.motion.yrel * 0.06);

			Mouse::resetToWindowCenter();

			m_camera.update();
		}
	}

	m_hotbar.onEvent(event);
	m_blockCursor.onEvent(event, m_hotbar);
}

void GameState::update() {
	Shader::bind(&m_shader);
	m_shader.setUniform("u_time", GameClock::getTicks());
	Shader::bind(nullptr);

	m_world.updateChunks();

	m_viewMatrix = m_camera.processInputs(m_world);

	m_blockCursor.update(false);
}

void GameState::draw(RenderTarget &target, RenderStates states) const {
	states.shader = &m_shader;
	states.projectionMatrix = &m_projectionMatrix;
	states.viewMatrix = &m_viewMatrix;

	target.draw(m_world, states);
	target.draw(m_crosshair, states);
	target.draw(m_blockCursor, states);
	target.draw(m_hotbar, states);
}

