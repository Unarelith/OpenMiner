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

#include <gk/core/ApplicationStateStack.hpp>
#include <gk/core/input/GamePad.hpp>
#include <gk/core/Mouse.hpp>

#include "Config.hpp"
#include "KeyboardHandler.hpp"
#include "SettingsMenuState.hpp"
#include "World.hpp"

SettingsMenuState::SettingsMenuState(gk::ApplicationState *parent) : ApplicationState(parent) {
	m_shader.createProgram();
	m_shader.addShader(GL_VERTEX_SHADER, "resources/shaders/basic.v.glsl");
	m_shader.addShader(GL_FRAGMENT_SHADER, "resources/shaders/basic.f.glsl");
	m_shader.linkProgram();

	m_projectionMatrix = glm::ortho(0.0f, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, 0.0f);

	m_background.setColor(gk::Color{0, 0, 0, 127});
	m_background.setSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	m_menuWidget.setScale(GUI_SCALE, GUI_SCALE, 1);

	addMainButtons();
}

void SettingsMenuState::onEvent(const SDL_Event &event) {
	m_menuWidget.onEvent(event);

	if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
		m_stateStack->pop();
	}
	else if (m_currentKeyButton && event.type == SDL_KEYDOWN) {
		KeyboardHandler *keyboardHandler = (KeyboardHandler *)gk::GamePad::getInputHandler();
		keyboardHandler->setKeycode(m_currentKey, event.key.keysym.sym);

		m_currentKeyButton->setText(m_currentKeyButton->text() + keyboardHandler->getKeyName(m_currentKey));
		m_currentKeyButton = nullptr;
	}
}

void SettingsMenuState::update() {
}

void SettingsMenuState::addMainButtons() {
	m_menuWidget.addButton(0, 0, "Gameplay...", [this] (TextButton &) {
		m_menuWidget.reset(1, 8);
		addGameplayButtons();
	});

	m_menuWidget.addButton(0, 1, "Graphics...", [this] (TextButton &) {
		m_menuWidget.reset(1, 8);
		addGraphicsButtons();
	});

	m_menuWidget.addButton(0, 2, "Input...", [this] (TextButton &) {
		m_menuWidget.reset(1, 8);
		addInputButtons();
	});

	m_menuWidget.addButton(0, 7, "Done", [this] (TextButton &) { m_stateStack->pop(); });
}

void SettingsMenuState::addGameplayButtons() {
	addToggleButton(0, 0, "Fly Mode", Config::isFlyModeEnabled, false);
	addToggleButton(0, 1, "No Clip", Config::isNoClipEnabled, false);

	m_menuWidget.addButton(0, 7, "Done", [this] (TextButton &) {
		m_menuWidget.reset(1, 8);
		addMainButtons();
	});
}

void SettingsMenuState::addGraphicsButtons() {
	m_menuWidget.addButton(0, 0, "Render Distance: " + std::to_string(Config::renderDistance), [] (TextButton &button) {
		Config::renderDistance = std::max(4, (Config::renderDistance + 2) % 16);
		button.setText("Render Distance: " + std::to_string(Config::renderDistance));
		World::isReloadRequested = true;
	});

	addToggleButton(0, 1, "Smooth Lighting", Config::isSmoothLightingEnabled, true);
	addToggleButton(0, 2, "Ambient Occlusion", Config::isAmbientOcclusionEnabled, true);
	addToggleButton(0, 3, "Wireframe Mode", Config::isWireframeModeEnabled, false);

	m_menuWidget.addButton(0, 4, "GUI Scale: " + std::to_string(GUI_SCALE), [] (TextButton &button) {
		GUI_SCALE = 1 + (GUI_SCALE + 1) % 3;
		button.setText("GUI Scale: " + std::to_string(GUI_SCALE));
		// FIXME: Fix decrease bug
		//        Reload menus with new scaling
	});

	m_menuWidget.addButton(0, 5, "Fullscreen: OFF", [] (TextButton &) {}).setEnabled(false);
	m_menuWidget.addButton(0, 6, "Use VSync: OFF", [] (TextButton &) {}).setEnabled(false);

	m_menuWidget.addButton(0, 7, "Done", [this] (TextButton &) {
		m_menuWidget.reset(1, 8);
		addMainButtons();
	});
}

void SettingsMenuState::addInputButtons() {
	std::vector<std::pair<u8, std::string>> keys = {
		{GameKey::Up,        "Forward"},
		{GameKey::Down,      "Back"},
		{GameKey::Left,      "Left"},
		{GameKey::Right,     "Right"},
		{GameKey::Jump,      "Jump"},
		{GameKey::Fly,       "Jetpack"},
		// {GameKey::Sneak,     "Sneak"},
		// {GameKey::Sprint,    "Sprint"},
		// {GameKey::Dig,       "Dig"},
		// {GameKey::Use,       "Use"},
		{GameKey::Inventory, "Inventory"},
	};

	KeyboardHandler *keyboardHandler = (KeyboardHandler *)gk::GamePad::getInputHandler();
	int i = 0;
	for (auto &it : keys) {
		m_menuWidget.addButton(0, i++, it.second + ": " + keyboardHandler->getKeyName(it.first), [this, it] (TextButton &button) {
			button.setText(it.second + ": ");
			m_currentKey = it.first;
			m_currentKeyButton = &button;
		});
	}

	m_menuWidget.addButton(0, 7, "Done", [this] (TextButton &) {
		m_menuWidget.reset(1, 8);
		addMainButtons();
	});
}

void SettingsMenuState::addToggleButton(u16 x, u16 y, const std::string &text, bool &configOption, bool worldReloadRequested) {
	m_menuWidget.addButton(x, y, text + ": " + (configOption ? "ON" : "OFF"), [=, &configOption] (TextButton &button) {
		configOption = !configOption;
		button.setText(text + ": " + (configOption ? "ON" : "OFF"));

		if (worldReloadRequested)
			World::isReloadRequested = true;
	});
}

void SettingsMenuState::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	if (m_parent)
		target.draw(*m_parent, states);

	states.transform *= getTransform();

	states.projectionMatrix = m_projectionMatrix;

	states.shader = &m_shader;
	states.vertexAttributes = gk::VertexAttribute::Only2d;

	target.draw(m_background, states);
	target.draw(m_menuWidget, states);
}

