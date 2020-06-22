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
#include <gk/core/Utils.hpp>

#include <filesystem.hpp>

#include "Config.hpp"
#include "TitleScreenState.hpp"
#include "WorldDeletionState.hpp"
#include "WorldInfoState.hpp"
#include "WorldSelectionState.hpp"

namespace fs = ghc::filesystem;

WorldInfoState::WorldInfoState(const std::string &worldName, TitleScreenState *titleScreen) : InterfaceState(titleScreen) {
	m_textInput.setString(worldName);
	m_textInput.setCharacterLimit(32);
	m_textInput.setBackgroundSize(150, 20);
	m_textInput.setBackgroundOutline(1, gk::Color::White);
	m_textInput.setPadding(5, 6);
	m_textInput.setScale(Config::guiScale, Config::guiScale);

	m_playButton.setText("Play");
	m_playButton.setScale(Config::guiScale, Config::guiScale);
	m_playButton.setCallback([this, worldName, titleScreen](TextButton &) {
		if (fs::exists("saves/" + worldName + ".dat")) {
			m_stateStack->pop();
			titleScreen->startSingleplayer(true, worldName);
		}
		else {
			m_errorText.setString("World '" + worldName + "' doesn't exist");
			m_errorText.updateVertexBuffer();
			updateWidgetPosition();
		}
	});

	m_renameButton.setText("Rename");
	m_renameButton.setScale(Config::guiScale, Config::guiScale);
	m_renameButton.setCallback([this, titleScreen, world=worldName](TextButton &) {
		std::string worldName = m_textInput.string();
		if (!fs::exists("saves/" + worldName + ".dat")) {
			if (gk::regexMatch(worldName, "^[A-Za-z0-9_]+$")) {
				fs::copy_file("saves/" + world + ".dat", "saves/" + worldName + ".dat");
				fs::remove("saves/" + world + ".dat");

				m_stateStack->pop();

				// FIXME: This is needed because there's currently no way to refresh WorldSelectionState
				m_stateStack->pop(); // WorldSelectionState
				m_stateStack->push<WorldSelectionState>(titleScreen);
			}
			else {
				m_errorText.setString("Invalid world name");
				m_errorText.updateVertexBuffer();
				updateWidgetPosition();
			}
		}
		else {
			m_errorText.setString("World '" + worldName + "' already exists");
			m_errorText.updateVertexBuffer();
			updateWidgetPosition();
		}
	});

	m_cancelButton.setText("Cancel");
	m_cancelButton.setScale(Config::guiScale, Config::guiScale);
	m_cancelButton.setCallback([this](TextButton &) {
		m_stateStack->pop();
	});

	m_deleteButton.setText("Delete");
	m_deleteButton.setScale(Config::guiScale, Config::guiScale);
	m_deleteButton.setCallback([this, worldName, titleScreen](TextButton &) {
		m_stateStack->push<WorldDeletionState>(worldName, titleScreen);
	});

	m_errorText.setColor(gk::Color::Red);
	m_errorText.setScale(Config::guiScale, Config::guiScale);

	updateWidgetPosition();
}

void WorldInfoState::onEvent(const sf::Event &event) {
	InterfaceState::onEvent(event);

	if (event.type == sf::Event::Resized) {
		updateWidgetPosition();
	}

	if (!m_stateStack->empty() && &m_stateStack->top() == this) {
		m_textInput.onEvent(event);

		m_playButton.onEvent(event);
		m_renameButton.onEvent(event);
		m_cancelButton.onEvent(event);
		m_deleteButton.onEvent(event);
	}
}

void WorldInfoState::update() {
}

void WorldInfoState::updateWidgetPosition() {
	m_textInput.setPosition(
		Config::screenWidth / 2.0f - m_textInput.getBackgroundSize().x * Config::guiScale / 2.0f,
		Config::screenHeight / 2.0f - m_textInput.getBackgroundSize().y * Config::guiScale / 2.0f
	);

	m_playButton.setPosition(Config::screenWidth / 2.0f - m_playButton.getGlobalBounds().sizeX / 2, Config::screenHeight - 420);
	m_renameButton.setPosition(Config::screenWidth / 2.0f - m_playButton.getGlobalBounds().sizeX / 2, Config::screenHeight - 340);
	m_deleteButton.setPosition(Config::screenWidth / 2.0f - m_deleteButton.getGlobalBounds().sizeX / 2, Config::screenHeight - 260);
	m_cancelButton.setPosition(Config::screenWidth / 2.0f - m_cancelButton.getGlobalBounds().sizeX / 2, Config::screenHeight - 180);

	m_errorText.setPosition(
		Config::screenWidth / 2.0f - m_errorText.getSize().x * Config::guiScale / 2.0f,
		Config::screenHeight / 2.0f - 30 * Config::guiScale
	);
}

void WorldInfoState::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	if (m_parent)
		target.draw(*m_parent, states);

	if (&m_stateStack->top() == this) {
		prepareDraw(target, states);

		target.draw(m_textInput, states);

		target.draw(m_playButton, states);
		target.draw(m_renameButton, states);
		target.draw(m_cancelButton, states);
		target.draw(m_deleteButton, states);

		if (!m_errorText.string().empty())
			target.draw(m_errorText, states);
	}
}
