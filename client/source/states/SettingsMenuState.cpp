/*
 * =====================================================================================
 *
 *  OpenMiner
 *  Copyright (C) 2018-2020 Unarelith, Quentin Bazin <openminer@unarelith.net>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *
 * =====================================================================================
 */
#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>

#include <gk/core/input/GamePad.hpp>
#include <gk/core/input/KeyboardHandler.hpp>
#include <gk/core/ApplicationStateStack.hpp>
#include <gk/core/Debug.hpp>
#include <gk/core/Mouse.hpp>

#include "Config.hpp"
#include "SettingsMenuState.hpp"
#include "World.hpp"

SettingsMenuState::SettingsMenuState(gk::ApplicationState *parent) : InterfaceState(parent) {
	m_menuWidget.setScale(Config::guiScale, Config::guiScale, 1);

	m_doneButton.setPosition(Config::screenWidth / 2.0f - m_doneButton.getGlobalBounds().width * Config::guiScale / 2.0f, Config::screenHeight - 291);
	m_doneButton.setScale(Config::guiScale, Config::guiScale, 1);
	m_doneButton.setText("Done");
	m_doneButton.setCallback([this] (TextButton &) {
		doneButtonAction();
	});

	addMainButtons();
}

void SettingsMenuState::onEvent(const SDL_Event &event) {
	InterfaceState::onEvent(event);

	if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
		m_doneButton.setPosition(Config::screenWidth / 2.0f - m_doneButton.getGlobalBounds().width / 2.0f, Config::screenHeight - 291);

		if (&m_stateStack->top() != this)
			m_menuWidget.onEvent(event);
	}

	if (&m_stateStack->top() == this) {
		m_menuWidget.onEvent(event);
		m_doneButton.onEvent(event);

		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
			doneButtonAction();
		}
		else if (m_currentKeyButton && event.type == SDL_KEYDOWN) {
			gk::KeyboardHandler *keyboardHandler = (gk::KeyboardHandler *)gk::GamePad::getInputHandler();
			keyboardHandler->setKeycode(m_currentKey, event.key.keysym.sym);

			m_currentKeyButton->setText(m_currentKeyButton->text() + keyboardHandler->getKeyName(m_currentKey));
			m_currentKeyButton = nullptr;
		}
	}
}

void SettingsMenuState::update() {
}

void SettingsMenuState::doneButtonAction() {
	if (m_state != MenuState::Main) {
		m_state = MenuState::Main;
		addMainButtons();
	} else {
		m_stateStack->pop();
	}
}

void SettingsMenuState::addMainButtons() {
	m_menuWidget.reset(1, 8);

	m_menuWidget.addButton("Gameplay...", [this] (TextButton &) {
		m_state = MenuState::Gameplay;
		addGameplayButtons();
	});

	m_menuWidget.addButton("Graphics...", [this] (TextButton &) {
		m_state = MenuState::Graphics;
		addGraphicsButtons();
	});

	m_menuWidget.addButton("Input...", [this] (TextButton &) {
		m_state = MenuState::Input;
		addInputButtons();
	});
}

void SettingsMenuState::addGameplayButtons() {
	m_menuWidget.reset(1, 8);

	addToggleButton("Fly Mode", Config::isFlyModeEnabled, false);
	addToggleButton("No Clip", Config::isNoClipEnabled, false);
}

void SettingsMenuState::addGraphicsButtons() {
	m_menuWidget.reset(2, 8);

	m_menuWidget.addButton("Render Distance: " + std::to_string(Config::renderDistance), [] (TextButton &button) {
		Config::renderDistance = std::max(4, (Config::renderDistance + 2) % 16);
		button.setText("Render Distance: " + std::to_string(Config::renderDistance));
		World::isReloadRequested = true;
	});

	addToggleButton("Wireframe Mode", Config::isWireframeModeEnabled, false);

	addToggleButton("Torch Smooth Lighting", Config::isTorchSmoothLightingEnabled, true);
	addToggleButton("Sun Smooth Lighting", Config::isSunSmoothLightingEnabled, true);
	addToggleButton("Ambient Occlusion", Config::isAmbientOcclusionEnabled, false);

	m_menuWidget.addButton("GUI Scale: " + std::to_string(Config::guiScale), [] (TextButton &button) {
		Config::guiScale = 1 + (Config::guiScale + 1) % 3;
		button.setText("GUI Scale: " + std::to_string(Config::guiScale));
	});

	addToggleButton("Fullscreen", Config::isFullscreenModeEnabled, false);
	m_menuWidget.addButton("Resolution: " + std::to_string(Config::screenWidth) + "x" + std::to_string(Config::screenHeight), [] (TextButton &button) {
		if (Config::isFullscreenModeEnabled) return;

		// FIXME: Find a better way to do this
		if (Config::screenWidth == 1600 && Config::screenHeight == 1050) {
			Config::screenWidth = 1280;
			Config::screenHeight = 720;
		}
		else if (Config::screenWidth == 1280 && Config::screenHeight == 720) {
			Config::screenWidth = 1920;
			Config::screenHeight = 1080;
		}
		else {
			Config::screenWidth = 1600;
			Config::screenHeight = 1050;
		}

		button.setText("Resolution: " + std::to_string(Config::screenWidth) + "x" + std::to_string(Config::screenHeight));
	});

	m_menuWidget.addButton("Use VSync: ON", [] (TextButton &) {}).setEnabled(false);
}

void SettingsMenuState::addInputButtons() {
	std::vector<std::pair<u8, std::string>> keys = {
		{GameKey::Up,        "Forward"},
		{GameKey::Left,      "Left"},
		{GameKey::Down,      "Back"},
		{GameKey::Right,     "Right"},
		{GameKey::Jump,      "Jump"},
		{GameKey::Fly,       "Jetpack"},
		{GameKey::Sneak,     "Sneak"},
		{GameKey::Sprint,    "Sprint"},
		// {GameKey::Dig,       "Dig"},
		// {GameKey::Use,       "Use"},
		{GameKey::Inventory, "Inventory"},
	};

	m_menuWidget.reset(2, 8);

	gk::KeyboardHandler *keyboardHandler = (gk::KeyboardHandler *)gk::GamePad::getInputHandler();
	for (auto &it : keys) {
		m_menuWidget.addButton(it.second + ": " + keyboardHandler->getKeyName(it.first), [this, it] (TextButton &button) {
			button.setText(it.second + ": ");
			m_currentKey = it.first;
			m_currentKeyButton = &button;
		});
	}

	m_menuWidget.addButton("Mouse sensitivity: " + std::to_string(Config::mouseSensitivity), [] (TextButton &button) {
		Config::mouseSensitivity = std::max(2, (Config::mouseSensitivity + 2) % 14);
		button.setText("Mouse sensitivity: " + std::to_string(Config::mouseSensitivity));
	});
}

TextButton &SettingsMenuState::addToggleButton(const std::string &text, bool &configOption, bool worldReloadRequested) {
	return m_menuWidget.addButton(text + ": " + (configOption ? "ON" : "OFF"), [=, &configOption] (TextButton &button) {
		configOption = !configOption;
		button.setText(text + ": " + (configOption ? "ON" : "OFF"));

		if (worldReloadRequested)
			World::isReloadRequested = true;
	});
}

void SettingsMenuState::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	if (m_parent)
		target.draw(*m_parent, states);

	prepareDraw(target, states);

	target.draw(m_menuWidget, states);
	target.draw(m_doneButton, states);

	target.disableView();
}

