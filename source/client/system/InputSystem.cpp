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
#include "HUD.hpp"
#include "InputSystem.hpp"
#include "KeyboardHandler.hpp"
#include "MessageBus.hpp"
#include "Skybox.hpp"

#include "GameplaySystem.hpp"
#include "RenderingSystem.hpp"

void InputSystem::onEvent(const SDL_Event &event) {
	KeyboardHandler *keyboardHandler = (KeyboardHandler *)gk::GamePad::getInputHandler();

	if (event.type == SDL_MOUSEMOTION) {
		m_messageBus.publish<GameplayEvent::RotateCamera>(
			event.motion.x, event.motion.y,
			event.motion.xrel, event.motion.yrel
		);
	}
	else if (event.type == SDL_WINDOWEVENT) {
		if (event.window.event == SDL_WINDOWEVENT_FOCUS_LOST) {
			m_messageBus.publish<GameplayEvent::PauseGame>(m_stateStack, m_currentState);

			gk::Mouse::setCursorGrabbed(false);
			gk::Mouse::setCursorVisible(true);
		}
		else if (event.type == SDL_WINDOWEVENT_FOCUS_GAINED) {
			gk::Mouse::setCursorGrabbed(true);
			gk::Mouse::setCursorVisible(false);
		}
		else if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
			Config::screenWidth = (u16)event.window.data1;
			Config::screenHeight = (u16)event.window.data2;

			m_messageBus.publish<RenderingEvent::WindowSizeChanged>(
				Config::screenWidth,
				Config::screenHeight
			);
		}
	}
	else if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_ESCAPE) {
			m_messageBus.publish<GameplayEvent::PauseGame>(m_stateStack, m_currentState);
		}
		if (event.key.keysym.sym == keyboardHandler->getKeycode(GameKey::Chat)) {
			m_messageBus.publish<GameplayEvent::OpenChat>(false, m_stateStack, m_currentState);
		}
		else if (event.key.keysym.sym == keyboardHandler->getKeycode(GameKey::Command)) {
			m_messageBus.publish<GameplayEvent::OpenChat>(true, m_stateStack, m_currentState);
		}
		else if (event.key.keysym.sym == keyboardHandler->getKeycode(GameKey::BlockInfoToggle)) {
			Config::isBlockInfoWidgetEnabled = !Config::isBlockInfoWidgetEnabled;
		}
		else if (event.key.keysym.sym == SDLK_F2) {
			m_messageBus.publish<GameplayEvent::TakeScreenshot>();
		}

		// Forward key press events to server
		for (auto &key : Registry::getInstance().keys()) {
			if (event.key.keysym.sym == key.keycode()) {
				m_clientCommandHandler.sendKeyPressed(key.id());
			}
		}
	}

	m_hud.onEvent(event);
}

void InputSystem::update() {
	// Initialize mod-defined key bindings if needed
	if (!m_areModKeysLoaded) {
		KeyboardHandler *keyboardHandler = (KeyboardHandler *)gk::GamePad::getInputHandler();

		for (auto &it : Registry::getInstance().keys()) {
			keyboardHandler->addKey(it.id(), it.name(), it.keycode(), it.stringID(), &it);
		}

		m_areModKeysLoaded = true;
	}

	bool allowWorldReload = !m_stateStack->empty()
		&& (&m_stateStack->top() == m_currentState
		  || m_stateStack->top().parent() == m_currentState);

	bool processPlayerInputs = !m_stateStack->empty() && &m_stateStack->top() == m_currentState;
	bool sendPlayerPosRotUpdate = gk::GameClock::getInstance().getTicks() % 100 < 10;

	m_messageBus.publish<GameplayEvent::GameUpdate>(
		allowWorldReload,
		processPlayerInputs,
		sendPlayerPosRotUpdate
	);
}

