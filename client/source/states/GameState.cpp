/*
 * =====================================================================================
 *
 *  OpenMiner
 *
 *  Copyright (C) 2018-2020 Unarelith, Quentin Bazin <openminer@unarelith.net>
 *  Copyright (C) 2019-2020 the OpenMiner contributors (see CONTRIBUTORS.md)
 *
 *  This file is part of OpenMiner.
 *
 *  OpenMiner is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  OpenMiner is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with OpenMiner; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
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
#include <gk/core/input/KeyboardHandler.hpp>
#include <gk/gl/OpenGL.hpp>
#include <gk/resource/ResourceHandler.hpp>

#include "ChatState.hpp"
#include "GameKey.hpp"
#include "GameState.hpp"
#include "LuaGUIState.hpp"
#include "PauseMenuState.hpp"
#include "Registry.hpp"
#include "TextureAtlas.hpp"

GameState::GameState(const std::string &host, int port) {
	// Set clear color to skyblue
	glClearColor(0.196078, 0.6, 0.8, 1.0);

	m_textureAtlas = &gk::ResourceHandler::getInstance().get<TextureAtlas>("atlas-blocks");

	m_camera.setAspectRatio((float)Config::screenWidth / Config::screenHeight);

	initShaders();

	gk::Mouse::setCursorVisible(false);
	gk::Mouse::setCursorGrabbed(true);

	// m_playerBoxes.emplace(0, PlayerBox{});
	// m_playerBoxes.at(0).setPosition(0, 22, 35);

	m_client.connect(host, port);

	m_clientCommandHandler.setupCallbacks();

	m_world.setClient(m_clientCommandHandler);
	m_player.setClientID(m_client.id());
	m_world.setCamera(m_player.camera());
}

void GameState::onEvent(const SDL_Event &event) {
	if (event.type == SDL_QUIT) {
		m_client.disconnect();

		m_stateStack->clear();
	}

	if (!m_stateStack->empty() && &m_stateStack->top() == this) {
		gk::KeyboardHandler *keyboardHandler = (gk::KeyboardHandler *)gk::GamePad::getInputHandler();

		if (event.type == SDL_MOUSEMOTION) {
			if(Config::screenWidth / 2.0f != event.motion.x || Config::screenHeight / 2.0f != event.motion.y) {
				m_player.turnH(event.motion.xrel * -0.01 * Config::mouseSensitivity);
				m_player.turnViewV(event.motion.yrel * -0.01 * Config::mouseSensitivity);

				gk::Mouse::resetToWindowCenter();
			}
		}
		else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
			m_stateStack->push<PauseMenuState>(m_client, this);
		}
		else if (event.type == SDL_KEYDOWN
		&& (event.key.keysym.sym == keyboardHandler->getKeycode(GameKey::Chat)
		 || event.key.keysym.sym == keyboardHandler->getKeycode(GameKey::Command)))
		{
			m_stateStack->push<ChatState>(m_clientCommandHandler, m_hud.chat(), event.key.keysym.sym == keyboardHandler->getKeycode(GameKey::Command), this);
		}
		else if (event.type == SDL_WINDOWEVENT) {
			if (event.window.event == SDL_WINDOWEVENT_FOCUS_LOST) {
				m_stateStack->push<PauseMenuState>(m_client, this);

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

	if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
		Config::screenWidth = event.window.data1;
		Config::screenHeight = event.window.data2;

		m_camera.setAspectRatio((float)Config::screenWidth / Config::screenHeight);
		m_hud.setup();
	}
}

void GameState::update() {
	m_world.checkPlayerChunk(m_player.x(), m_player.y(), m_player.z());
	m_world.update();

	if (m_camera.getFieldOfView() != Config::cameraFOV)
		m_camera.setFieldOfView(Config::cameraFOV);

	// FIXME: Registry init and TextureAtlas building should be done during loading phase
	if (m_clientCommandHandler.isRegistryInitialized()) {
		if (m_textureAtlas->isReady()) {
			if (!m_stateStack->empty() && &m_stateStack->top() == this) {
				m_player.processInputs();

				if (gk::GamePad::isKeyPressedOnce(GameKey::Inventory)) {
					m_clientCommandHandler.sendPlayerInventoryRequest();
				}
			}

			m_player.updatePosition(m_world);

			m_hud.update();
		}
		else {
			m_textureAtlas->loadFromRegistry();
		}
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
	// FIXME: This uniform is not used anymore since water/leaves effects are disabled
	// gk::Shader::bind(&m_shader);
	// m_shader.setUniform("u_time", gk::GameClock::getTicks());
	// gk::Shader::bind(nullptr);

	states.shader = &m_shader;

	target.setView(m_camera);
	target.draw(m_world, states);

	for (auto &it : m_playerBoxes)
		target.draw(it.second, states);

	if (m_clientCommandHandler.isRegistryInitialized())
		target.draw(m_hud, states);
}

