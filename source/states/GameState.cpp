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

#include "ApplicationStateStack.hpp"
#include "Config.hpp"
#include "GameClock.hpp"
#include "GameState.hpp"
#include "InventoryState.hpp"
#include "Keyboard.hpp"
#include "Mouse.hpp"
#include "PauseMenuState.hpp"
#include "PlayerInventoryWidget.hpp"
#include "ScriptEngine.hpp"

GameState::GameState() {
	m_projectionMatrix = glm::perspective(45.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, DIST_NEAR, DIST_FAR);

	auto &lua = ScriptEngine::getInstance().lua();
	lua["player"] = &m_player;
	lua["init"]();

	initShaders();
}

void GameState::onEvent(const sf::Event &event) {
	static sf::Vector2i lastPosition;

	if (event.type == sf::Event::MouseMoved) {
		if(SCREEN_WIDTH / 2 != event.mouseMove.x || SCREEN_HEIGHT / 2 != event.mouseMove.y) {
			m_camera.turnH((event.mouseMove.x-lastPosition.x) * 0.06);
			m_camera.turnV(-(event.mouseMove.y-lastPosition.y) * 0.06);

			auto midPos = Mouse::resetToWindowCenter();
			lastPosition = {midPos.x, midPos.y};

			m_camera.updateViewMatrix();
		}
	}
	else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape && &m_stateStack->top() == this) {
		m_stateStack->push<PauseMenuState>(this);
	}
	//When window is not in focus SFML still receives Mouse movement and sf::Keyboard::isKeyPressed() still returns true
	//To fix, push Pause state. Alternative: check if window has focus and only handle input if it does (Need access to Window somehow...)
	else if (event.type == sf::Event::LostFocus) {
		m_stateStack->push<PauseMenuState>(this);

		Mouse::setCursorGrabbed(false);
		Mouse::setCursorVisible(true);
	}
	else if (event.type == sf::Event::GainedFocus) {
		Mouse::setCursorGrabbed(true);
		Mouse::setCursorVisible(false);
	}

	m_hud.onEvent(event);
}

void GameState::update() {
	Shader::bind(&m_shader);
	m_shader.setUniform("u_time", GameClock::getTicks());
	Shader::bind(nullptr);

	m_world.update(m_player);

	if (&m_stateStack->top() == this) {
		m_camera.processInputs();

		if (Keyboard::isKeyPressedOnce(Keyboard::E)) {
			auto &inventoryState = m_stateStack->push<InventoryState>(this);
			inventoryState.setupWidget<PlayerInventoryWidget>(m_player.inventory());
		}
	}

	m_viewMatrix = m_camera.updatePosition(m_world);

	m_hud.update();
}

void GameState::initShaders() {
	m_shader.createProgram();

	m_shader.addShader(GL_VERTEX_SHADER, "resources/shaders/game.v.glsl");
	m_shader.addShader(GL_FRAGMENT_SHADER, "resources/shaders/color.f.glsl");
	m_shader.addShader(GL_FRAGMENT_SHADER, "resources/shaders/light.f.glsl");
	m_shader.addShader(GL_FRAGMENT_SHADER, "resources/shaders/fog.f.glsl");
	m_shader.addShader(GL_FRAGMENT_SHADER, "resources/shaders/game.f.glsl");

	m_shader.linkProgram();
}

void GameState::draw(RenderTarget &target, RenderStates states) const {
	states.shader = &m_shader;
	states.projectionMatrix = m_projectionMatrix;
	states.viewMatrix = m_viewMatrix;

	target.draw(m_world, states);
	target.draw(m_hud, states);
}

