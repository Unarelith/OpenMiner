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
#include <gk/core/GameClock.hpp>
#include <gk/core/input/GamePad.hpp>
#include <gk/core/Mouse.hpp>

#include "Client.hpp"
#include "ClientCommandHandler.hpp"
#include "ClientPlayer.hpp"
#include "ClientWorld.hpp"
#include "GameKey.hpp"
#include "InputSystem.hpp"
#include "KeyboardHandler.hpp"
#include "Skybox.hpp"

#include "ChatState.hpp"
#include "PauseMenuState.hpp"

void InputSystem::onEvent(const SDL_Event &event, gk::ApplicationStateStack *stateStack, gk::ApplicationState *currentState) {
	KeyboardHandler *keyboardHandler = (KeyboardHandler *)gk::GamePad::getInputHandler();

	if (event.type == SDL_MOUSEMOTION) {
		if(Config::screenWidth / 2 != event.motion.x || Config::screenHeight / 2 != event.motion.y) {
			m_player.turnH((float)event.motion.xrel * -0.01f * Config::mouseSensitivity);
			m_player.turnViewV((float)event.motion.yrel * -0.01f * Config::mouseSensitivity);

			gk::Mouse::resetToWindowCenter();
		}
	}
	else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
		stateStack->push<PauseMenuState>(m_client, currentState);
	}
	else if (event.type == SDL_WINDOWEVENT) {
		if (event.window.event == SDL_WINDOWEVENT_FOCUS_LOST) {
			stateStack->push<PauseMenuState>(m_client, currentState);

			gk::Mouse::setCursorGrabbed(false);
			gk::Mouse::setCursorVisible(true);
		}
		else if (event.type == SDL_WINDOWEVENT_FOCUS_GAINED) {
			gk::Mouse::setCursorGrabbed(true);
			gk::Mouse::setCursorVisible(false);
		}
	}
	else if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == keyboardHandler->getKeycode(GameKey::Chat)
		 || event.key.keysym.sym == keyboardHandler->getKeycode(GameKey::Command)) {
			stateStack->push<ChatState>(m_clientCommandHandler, m_hud.chat(), event.key.keysym.sym == keyboardHandler->getKeycode(GameKey::Command), currentState);
		}
		else if (event.key.keysym.sym == keyboardHandler->getKeycode(GameKey::BlockInfoToggle)) {
			Config::isBlockInfoWidgetEnabled = !Config::isBlockInfoWidgetEnabled;
		}
		else if (event.key.keysym.sym == SDLK_F2) {
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
}

void InputSystem::update(gk::ApplicationStateStack *stateStack, gk::ApplicationState *currentState) {
	m_world.checkPlayerChunk(m_player.x(), m_player.y(), m_player.z());
	m_world.update(!stateStack->empty() && (&stateStack->top() == currentState || stateStack->top().parent() == currentState));

	if (m_camera.getFieldOfView() != Config::cameraFOV)
		m_camera.setFieldOfView(Config::cameraFOV);

	if (!stateStack->empty() && &stateStack->top() == currentState) {
		m_player.processInputs();
	}

	if (!m_areModKeysLoaded) {
		KeyboardHandler *keyboardHandler = (KeyboardHandler *)gk::GamePad::getInputHandler();

		for (auto &it : Registry::getInstance().keys()) {
			keyboardHandler->addKey(it.id(), it.name(), it.keycode(), it.stringID(), &it);
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

