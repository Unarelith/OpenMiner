/*
 * =====================================================================================
 *
 *       Filename:  SettingsMenuState.cpp
 *
 *    Description:
 *
 *        Created:  29/06/2018 06:11:57
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
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
	m_background.setFillColor(gk::Color{0, 0, 0, 127});
	m_background.setSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	m_menuWidget.setScale(GUI_SCALE, GUI_SCALE, 1);

	m_doneButton.setPosition(SCREEN_WIDTH / 2 - m_doneButton.getGlobalBounds().width * GUI_SCALE / 2, SCREEN_HEIGHT - 291);
	m_doneButton.setScale(GUI_SCALE, GUI_SCALE, 1);
	m_doneButton.setText("Done");

	addMainButtons();
}

void SettingsMenuState::onEvent(const SDL_Event &event) {
	m_menuWidget.onEvent(event);
	m_doneButton.onEvent(event);

	if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
		m_stateStack->pop();
	}
	else if (m_currentKeyButton && event.type == SDL_KEYDOWN) {
		gk::KeyboardHandler *keyboardHandler = (gk::KeyboardHandler *)gk::GamePad::getInputHandler();
		keyboardHandler->setKeycode(m_currentKey, event.key.keysym.sym);

		m_currentKeyButton->setText(m_currentKeyButton->text() + keyboardHandler->getKeyName(m_currentKey));
		m_currentKeyButton = nullptr;
	}
}

void SettingsMenuState::update() {
}

void SettingsMenuState::addMainButtons() {
	m_menuWidget.reset(1, 8);

	m_menuWidget.addButton("Gameplay...", [this] (TextButton &) {
		addGameplayButtons();
	});

	m_menuWidget.addButton("Graphics...", [this] (TextButton &) {
		addGraphicsButtons();
	});

	m_menuWidget.addButton("Input...", [this] (TextButton &) {
		addInputButtons();
	});

	m_doneButton.setCallback([this] (TextButton &) { m_stateStack->pop(); });
}

void SettingsMenuState::addGameplayButtons() {
	m_menuWidget.reset(1, 8);

	addToggleButton("Fly Mode", Config::isFlyModeEnabled, false);
	addToggleButton("No Clip", Config::isNoClipEnabled, false);

	m_doneButton.setCallback([this] (TextButton &) {
		addMainButtons();
	});
}

void SettingsMenuState::addGraphicsButtons() {
	m_menuWidget.reset(1, 8);

	m_menuWidget.addButton("Render Distance: " + std::to_string(Config::renderDistance), [] (TextButton &button) {
		Config::renderDistance = std::max(4, (Config::renderDistance + 2) % 16);
		button.setText("Render Distance: " + std::to_string(Config::renderDistance));
		World::isReloadRequested = true;
	});

	addToggleButton("Torch Smooth Lighting", Config::isTorchSmoothLightingEnabled, true);
	addToggleButton("Sun Smooth Lighting", Config::isSunSmoothLightingEnabled, true);
	addToggleButton("Ambient Occlusion", Config::isAmbientOcclusionEnabled, true).setEnabled(false);
	addToggleButton("Wireframe Mode", Config::isWireframeModeEnabled, false);

	m_menuWidget.addButton("GUI Scale: " + std::to_string(GUI_SCALE), [] (TextButton &button) {
		GUI_SCALE = 1 + (GUI_SCALE + 1) % 3;
		button.setText("GUI Scale: " + std::to_string(GUI_SCALE));
		// FIXME: Fix decrease bug
		//        Reload menus with new scaling
	});

	m_menuWidget.addButton("Fullscreen: OFF", [] (TextButton &) {}).setEnabled(false);
	m_menuWidget.addButton("Use VSync: OFF", [] (TextButton &) {}).setEnabled(false);

	m_doneButton.setCallback([this] (TextButton &) {
		addMainButtons();
	});
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

	m_doneButton.setCallback([this] (TextButton &) {
		addMainButtons();
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

	if (m_parent)
		target.draw(m_background, states);

	target.draw(m_menuWidget, states);
	target.draw(m_doneButton, states);

	target.disableView();
}

