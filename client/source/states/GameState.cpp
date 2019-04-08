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
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

#include <gk/core/input/GamePad.hpp>
#include <gk/core/ApplicationStateStack.hpp>
#include <gk/core/GameClock.hpp>
#include <gk/core/Exception.hpp>
#include <gk/core/Mouse.hpp>
#include <gk/gl/OpenGL.hpp>

#include "GameKey.hpp"
#include "GameState.hpp"
#include "InventoryState.hpp"
#include "LuaGUIState.hpp"
#include "PauseMenuState.hpp"
#include "PlayerInventoryWidget.hpp"
#include "Registry.hpp"

GameState::GameState(const std::string &host, int port) {
	m_camera.setAspectRatio((float)SCREEN_WIDTH / SCREEN_HEIGHT);

	initShaders();

	gk::Mouse::setCursorVisible(false);
	gk::Mouse::setCursorGrabbed(true);

	// m_playerBoxes.emplace(0, PlayerBox{});
	// m_playerBoxes.at(0).setPosition(0, 22, 35);

	m_client.connect(host, port);

	m_clientCommandHandler.setupCallbacks();

	m_world.setClient(m_clientCommandHandler);
	m_player.setClientID(m_client.id());
}

void GameState::onEvent(const SDL_Event &event) {
	if (event.type == SDL_MOUSEMOTION) {
		if(SCREEN_WIDTH / 2 != event.motion.x || SCREEN_HEIGHT / 2 != event.motion.y) {
			m_player.turnH(event.motion.xrel * 0.06);
			m_player.turnV(-event.motion.yrel * 0.06);

			gk::Mouse::resetToWindowCenter();
		}
	}
	else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE && &m_stateStack->top() == this) {
		m_stateStack->push<PauseMenuState>(this);
	}
	else if (event.type == SDL_WINDOWEVENT) {
		if (event.window.event == SDL_WINDOWEVENT_FOCUS_LOST) {
			// FIXME
			// m_stateStack->push<PauseMenuState>(this);

			gk::Mouse::setCursorGrabbed(false);
			gk::Mouse::setCursorVisible(true);
		}
		else if (event.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) {
			gk::Mouse::setCursorGrabbed(true);
			gk::Mouse::setCursorVisible(false);
		}
	}

	if (m_clientCommandHandler.isRegistryInitialized())
		m_hud.onEvent(event);
}

void GameState::update() {
	m_world.update();

	if (m_clientCommandHandler.isRegistryInitialized()) {
		if (&m_stateStack->top() == this) {
			m_player.processInputs();

			if (gk::GamePad::isKeyPressedOnce(GameKey::Inventory)) {
				auto &inventoryState = m_stateStack->push<InventoryState>(this);
				inventoryState.setupWidget<PlayerInventoryWidget>(m_clientCommandHandler, m_player.inventory());
			}
		}

		m_player.updatePosition(m_world);

		m_hud.update();
	}

	if (gk::GameClock::getTicks() % 1000 < 10) {
		m_clientCommandHandler.sendPlayerPosUpdate();
	}

	m_client.update();
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

void GameState::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	gk::Shader::bind(&m_shader);
	m_shader.setUniform("u_time", gk::GameClock::getTicks());
	gk::Shader::bind(nullptr);

	states.shader = &m_shader;

	target.setView(m_camera);
	target.draw(m_world, states);

	for (auto &it : m_playerBoxes)
		target.draw(it.second, states);

	if (m_clientCommandHandler.isRegistryInitialized())
		target.draw(m_hud, states);
}

