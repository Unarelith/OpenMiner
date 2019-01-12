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
#include <gk/resource/ResourceHandler.hpp>

#include "GameKey.hpp"
#include "GameState.hpp"
#include "InventoryState.hpp"
#include "PauseMenuState.hpp"
#include "PlayerInventoryWidget.hpp"
#include "ScriptEngine.hpp"

GameState::GameState() : m_chunk(0, 0, 2, gk::ResourceHandler::getInstance().get<gk::Texture>("texture-blocks")) {
	try {
		m_client.connect("localhost", 4242);
	}
	catch (const gk::Exception &e) {
		std::cerr << "Error " << e.what() << std::endl;
	}

	m_camera.setAspectRatio((float)SCREEN_WIDTH / SCREEN_HEIGHT);

	World::setInstance(m_world);

	testLuaAPI();

	initShaders();

	m_client.setCommandCallback(Network::Command::ChunkData, [](sf::Packet &packet) {
		std::string str;
		packet >> str;
		std::cout << str << std::endl;
	});

	for (u16 x = 0 ; x < CHUNK_WIDTH ; ++x) {
		for (u16 y = 0 ; y < CHUNK_HEIGHT ; ++y) {
			for (u16 z = 0 ; z < CHUNK_DEPTH ; ++z) {
				m_chunk.setBlock(x, y, z, (rand() % 2) * 2);
				m_chunk.lightmap().addSunlight(x, y, z, 15);
			}
		}
	}
}

void GameState::testLuaAPI() {
	// FIXME
	// m_luaCore.setPlayer(m_player);
	// m_luaCore.setWorld(m_world);
    //
	// try {
	// 	auto &lua = ScriptEngine::getInstance().lua();
	// 	lua["openminer"] = &m_luaCore;
	// 	lua.script("init()");
	// }
	// catch (const sol::error &e) {
	// 	std::cerr << e.what() << std::endl;
	// }
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
	gk::Shader::bind(&m_shader);
	m_shader.setUniform("u_time", gk::GameClock::getTicks());
	gk::Shader::bind(nullptr);

	m_world.update(m_player);

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

	m_chunk.update();

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
	states.shader = &m_shader;

	// FIXME: Should be in World
	gk::Shader::bind(states.shader);
	states.shader->setUniform("u_renderDistance", World::renderDistance * CHUNK_WIDTH);
	gk::Shader::bind(nullptr);

	target.setView(m_camera);
	target.draw(m_skybox, states);
	// FIXME
	// target.draw(m_world, states);
	target.draw(m_chunk, states);
	target.draw(m_hud, states);
}

