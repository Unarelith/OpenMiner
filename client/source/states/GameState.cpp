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

#define GLM_FORCE_RADIANS
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
#include "PauseMenuState.hpp"
#include "PlayerInventoryWidget.hpp"
#include "ScriptEngine.hpp"

GameState::GameState() {
	try {
		m_client.connect("localhost", 4242);
	}
	catch (const gk::Exception &e) {
		std::cerr << "Error " << e.what() << std::endl;
	}

	m_camera.setAspectRatio((float)SCREEN_WIDTH / SCREEN_HEIGHT);

	testLuaAPI();

	initShaders();

	m_client.setCommandCallback(Network::Command::ChunkData, [this](sf::Packet &packet) {
		m_world.receiveChunkData(packet);
	});
}

void GameState::testLuaAPI() {
	m_luaCore.setPlayer(m_player);
	// m_luaCore.setWorld(m_world);

	try {
		auto &lua = ScriptEngine::getInstance().lua();
		lua["openminer"] = &m_luaCore;
		lua.script("init()");
	}
	catch (const sol::error &e) {
		std::cerr << e.what() << std::endl;
	}
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
		if (event.window.event == SDL_WINDOWEVENT_LEAVE) {
			m_stateStack->push<PauseMenuState>(this);

			gk::Mouse::setCursorGrabbed(false);
			gk::Mouse::setCursorVisible(true);
		}
		else if (event.window.event == SDL_WINDOWEVENT_ENTER) {
			gk::Mouse::setCursorGrabbed(true);
			gk::Mouse::setCursorVisible(false);
		}
	}

	m_hud.onEvent(event);
}

void GameState::update() {
	m_world.update();

	if (&m_stateStack->top() == this) {
		m_player.processInputs();

		if (gk::GamePad::isKeyPressedOnce(GameKey::Inventory)) {
			auto &inventoryState = m_stateStack->push<InventoryState>(this);
			inventoryState.setupWidget<PlayerInventoryWidget>(m_player.inventory());
		}
	}

	m_player.updatePosition(m_world);
	m_skybox.update(m_player);

	m_hud.update();

	m_client.update(m_hasGameStarted);
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
	// target.draw(m_skybox, states);
	target.draw(m_world, states);
	target.draw(m_hud, states);
}

