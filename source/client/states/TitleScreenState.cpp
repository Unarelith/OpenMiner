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
#include <gk/core/ApplicationStateStack.hpp>

#include "Config.hpp"
#include "Events.hpp"
#include "GameConfig.hpp"
#include "GameState.hpp"
#include "ServerConnectState.hpp"
#include "ServerLoadingState.hpp"
#include "SettingsMenuState.hpp"
#include "TitleScreenState.hpp"
#include "WorldSelectionState.hpp"

#include "ServerApplication.hpp" // For ServerOnlineEvent

TitleScreenState::TitleScreenState(u16 port) : m_port(port) {
	glClearColor(0.0, 0.0, 0.0, 1.0);

	m_menuWidget.setScale(Config::guiScale, Config::guiScale);

	m_menuWidget.addButton("Singleplayer", [this] (TextButton &) {
		m_stateStack->push<WorldSelectionState>(this);
	});

	m_menuWidget.addButton("Multiplayer", [this] (TextButton &) {
		m_stateStack->push<ServerConnectState>(this);
	});

	m_menuWidget.addButton("Options...", [this] (TextButton &) {
		m_stateStack->push<SettingsMenuState>(this);
	});

	m_menuWidget.addButton("Exit", [this] (TextButton &) {
		m_stateStack->pop();
	});

	m_titleText.setScale(Config::guiScale * 4, Config::guiScale * 4);
	m_titleText.setString(APP_NAME);
	m_titleText.updateVertexBuffer();

	std::string versionString = APP_NAME;
	versionString.append(" ");
	versionString.append(1, '0' + VERSION_MAJOR);
	versionString.append(".");
	versionString.append(1, '0' + VERSION_MINOR);
	versionString.append(".");
	versionString.append(1, '0' + VERSION_PATCH);
	m_versionText.setString(versionString);
	m_versionText.setScale(Config::guiScale, Config::guiScale);
	m_versionText.updateVertexBuffer();

	m_copyrightText.setString("Copyright Unarelith and contributors");
	m_copyrightText.setScale(Config::guiScale, Config::guiScale);
	m_copyrightText.updateVertexBuffer();

	m_licenseText.setString("License: LGPL v2.1");
	m_licenseText.setScale(Config::guiScale, Config::guiScale);
	m_licenseText.updateVertexBuffer();

	updateWidgetPosition();
}

TitleScreenState::~TitleScreenState() {
	if (m_thread.joinable())
		m_thread.join();
}

void TitleScreenState::init() {
	m_eventHandler->addListener<GuiScaleChangedEvent>(&TitleScreenState::onGuiScaleChanged, this);
}

void TitleScreenState::onEvent(const SDL_Event &event) {
	InterfaceState::onEvent(event);

	if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
		if (!m_stateStack->empty() && &m_stateStack->top() != this)
			m_menuWidget.onEvent(event);
	}

	if (!m_stateStack->empty() && &m_stateStack->top() == this) {
		m_menuWidget.onEvent(event);
	}
}

void TitleScreenState::startSingleplayer(bool showLoadingState, const std::string &worldName) {
	GameConfig::worldName = worldName;

	auto &game = m_stateStack->push<GameState>();
	game.setSingleplayer(true);

	auto &serverLoadingState = m_stateStack->push<ServerLoadingState>(game, showLoadingState, "localhost", sf::Socket::AnyPort, this);
	serverLoadingState.setUsername(Config::defaultUsername);

	if (m_thread.joinable())
		m_thread.join();

	gk::LogLevel logLevel = gk::LoggerHandler::getInstance().maxLevel();
	m_thread = std::thread([this, worldName, logLevel] () {
		ServerApplication app{*m_eventHandler};

		if (!worldName.empty())
			app.setWorldName(worldName);

		app.setLogLevel(logLevel);
		app.setSingleplayer(true);
		app.setPort(sf::Socket::AnyPort);
		app.run();
	});
}

void TitleScreenState::startMultiplayer(const std::string &host) {
	auto &game = m_stateStack->push<GameState>();
	auto &serverLoadingState = m_stateStack->push<ServerLoadingState>(game, false, host, m_port, this);
	serverLoadingState.setUsername(Config::defaultUsername);
}

void TitleScreenState::onGuiScaleChanged(const GuiScaleChangedEvent &event) {
	m_menuWidget.setScale(event.guiScale, event.guiScale);
	m_menuWidget.onGuiScaleChanged(event);

	m_titleText.setScale(event.guiScale * 4, event.guiScale * 4);
	m_versionText.setScale(event.guiScale, event.guiScale);
	m_copyrightText.setScale(event.guiScale, event.guiScale);
	m_licenseText.setScale(event.guiScale, event.guiScale);

	updateWidgetPosition();
}

void TitleScreenState::updateWidgetPosition() {
	m_background.setPosition(
		roundf(Config::screenWidth / 2.0f - m_background.width() / 2.0f),
		roundf(Config::screenHeight / 2.0f - m_background.height() / 2.0f)
	);

	m_menuWidget.setPosition(
		roundf(Config::screenWidth / 2.0f - m_menuWidget.getGlobalBounds().sizeX / 2.0f),
		roundf(Config::screenHeight / 2.0f - m_menuWidget.getGlobalBounds().sizeY / 2.0f)
	);

	m_titleText.setPosition(
		roundf(Config::screenWidth / 2.0f - m_titleText.getSize().x * m_titleText.getScale().x / 2.0f),
		roundf(0.04 * Config::screenHeight * Config::guiScale)
	);
	m_versionText.setPosition(Config::guiScale, Config::screenHeight - m_versionText.getSize().y * Config::guiScale);
	m_copyrightText.setPosition(
		roundf(Config::screenWidth - m_copyrightText.getSize().x * Config::guiScale - Config::guiScale),
		roundf(Config::screenHeight - m_copyrightText.getSize().y * Config::guiScale)
	);
	m_licenseText.setPosition(
		roundf(Config::screenWidth - m_licenseText.getSize().x * Config::guiScale - Config::guiScale),
		roundf(Config::screenHeight - (m_copyrightText.getSize().y + m_licenseText.getSize().y + 2) * Config::guiScale)
	);
}

void TitleScreenState::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	prepareDraw(target, states);

	target.draw(m_background, states);

	target.draw(m_titleText, states);
	target.draw(m_versionText, states);
	target.draw(m_copyrightText, states);
	target.draw(m_licenseText, states);

	target.draw(m_menuWidget, states);
}

