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
#include <algorithm>
#include <cmath>
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

#include <gk/core/input/GamePad.hpp>
#include <gk/core/ApplicationStateStack.hpp>
#include <gk/core/GameClock.hpp>
#include <gk/core/Exception.hpp>
#include <gk/core/Mouse.hpp>
#include <gk/gl/OpenGL.hpp>
#include <gk/resource/ResourceHandler.hpp>

#include "ChatState.hpp"
#include "Events.hpp"
#include "GameKey.hpp"
#include "GameState.hpp"
#include "KeyboardHandler.hpp"
#include "LuaGUIState.hpp"
#include "PauseMenuState.hpp"
#include "Registry.hpp"
#include "TextureAtlas.hpp"

GameState::GameState()
	: m_textureAtlas(gk::ResourceHandler::getInstance().get<TextureAtlas>("atlas-blocks"))
{
	Registry::setInstance(m_registry);

	initShaders();

	m_clientCommandHandler.setupCallbacks();

	m_camera.setAspectRatio((float)Config::screenWidth / Config::screenHeight);
	m_camera.setFarClippingPlane(10000.0f);

	m_world.setClient(m_clientCommandHandler);
	m_world.setCamera(m_player.camera());

	m_keyboardHandler = dynamic_cast<KeyboardHandler *>(gk::GamePad::getInputHandler());
}

void GameState::init() {
	m_eventHandler->addListener<GuiScaleChangedEvent>(&GameState::onGuiScaleChanged, this);
}

void GameState::connect(const std::string &host, int port, const std::string &username) {
	m_player.setName(username.empty() ? "Player" : username);
	m_client.connect(host, port, m_player);
	m_player.setClientID(m_client.id());
	m_client.addPlayer(m_player);

	gk::Mouse::setCursorVisible(false);
	gk::Mouse::setCursorGrabbed(true);
}

void GameState::onEvent(const SDL_Event &event) {
	if (event.type == SDL_QUIT) {
		m_client.disconnect();

		m_stateStack->clear();
	}

	if (!m_stateStack->empty() && &m_stateStack->top() == this) {
		KeyboardHandler *keyboardHandler = (KeyboardHandler *)gk::GamePad::getInputHandler();

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
			else if (event.type == SDL_WINDOWEVENT_FOCUS_GAINED) {
				gk::Mouse::setCursorGrabbed(true);
				gk::Mouse::setCursorVisible(false);
			}
		}
		else if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym == SDLK_F2) {
				std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

				char filename[100];
				std::strftime(filename, sizeof(filename), "Screenshot-%Y-%m-%d-%H-%M-%S.png", std::localtime(&now));

				bool isScreenshotSaved = gk::Window::saveScreenshot(0, 0, Config::screenWidth, Config::screenHeight, filename);
				if (isScreenshotSaved)
					m_hud.chat().addChatMessage(0, "Screenshot saved: " + std::string(filename));
				else
					m_hud.chat().addChatMessage(0, "Failed to save screenshot");
			}

			for (auto &key : Registry::getInstance().keys()) {
				if (event.key.keysym.sym == key.keycode()) {
					m_clientCommandHandler.sendKeyPressed(key.id());
				}
			}
		}

		m_hud.onEvent(event);
	}

	if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
		Config::screenWidth = event.window.data1;
		Config::screenHeight = event.window.data2;

		m_camera.setAspectRatio((float)Config::screenWidth / Config::screenHeight);
		m_hud.setup();

		m_fbo.init(Config::screenWidth, Config::screenHeight);
	}
}

void GameState::update() {
	m_world.checkPlayerChunk(m_player.x(), m_player.y(), m_player.z());
	m_world.update(!m_stateStack->empty() && &m_stateStack->top() == this);

	if (m_camera.getFieldOfView() != Config::cameraFOV)
		m_camera.setFieldOfView(Config::cameraFOV);

	if (!m_stateStack->empty() && &m_stateStack->top() == this) {
		m_player.processInputs();
	}

	if (!m_areModKeysLoaded) {
		for (auto &it : Registry::getInstance().keys()) {
			m_keyboardHandler->addKey(it.id(), it.name(), it.keycode(), it.stringID(), &it);
		}

		m_areModKeysLoaded = true;
	}

	m_player.updatePosition(m_world);

	m_hud.update();

	if (gk::GameClock::getInstance().getTicks() % 100 < 10) {
		m_clientCommandHandler.sendPlayerPosUpdate();
		m_clientCommandHandler.sendPlayerRotUpdate();
	}

	m_client.update();
}

void GameState::initShaders() {
	m_shader.createProgram();
	m_shader.addShader(GL_VERTEX_SHADER, "resources/shaders/game.v.glsl");
	m_shader.addShader(GL_FRAGMENT_SHADER, "resources/shaders/light.f.glsl");
	m_shader.addShader(GL_FRAGMENT_SHADER, "resources/shaders/fog.f.glsl");
	m_shader.addShader(GL_FRAGMENT_SHADER, "resources/shaders/game.f.glsl");
	m_shader.linkProgram();

	m_fbo.loadShader("screen");
}

void GameState::onGuiScaleChanged(const GuiScaleChangedEvent &event) {
	m_hud.onGuiScaleChanged(event);
}

void GameState::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	float time = std::fmod(gk::GameClock::getInstance().getTicks() * 4.f / 1000.f, 360.f) / 360.f;
	if (m_world.sky()) {
		const float pi = 3.1415927f;
		float sunlight = std::clamp(0.5f + std::sin(2 * pi * time) * 2.0f, 0.0f, 1.0f);

		gk::Color skyColor = m_world.sky()->color();
		float red = std::clamp(sunlight - (1 - skyColor.r), 0.0f, skyColor.r);
		float green = std::clamp(sunlight - (1 - skyColor.g), 0.0f, skyColor.g);
		float blue = std::clamp(sunlight - (1 - skyColor.b), 0.0f, skyColor.b);

		glClearColor(red, green, blue, 1.0f);
	}

	gk::Shader::bind(&m_shader);
	m_shader.setUniform("u_time", time);
	gk::Shader::bind(nullptr);

	m_fbo.begin();

	states.shader = &m_shader;

	target.setView(m_camera);

	target.draw(m_skybox, states);
	target.draw(m_world, states);

	for (auto &it : m_playerBoxes)
		if (it.second.dimension() == m_player.dimension())
			target.draw(it.second, states);

	target.draw(m_hud.blockCursor(), states);

	m_fbo.end();

	target.draw(m_hud, states);
}

