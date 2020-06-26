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
#include "WorldCreationState.hpp"
#include "WorldSelectionState.hpp"

namespace fs = ghc::filesystem;

WorldCreationState::WorldCreationState(TitleScreenState *titleScreen, const std::string &originalName) : InterfaceState(titleScreen) {
	m_textInput.setString(originalName);
	m_textInput.setCharacterLimit(32);
	m_textInput.setBackgroundSize(150, 20);
	m_textInput.setBackgroundOutline(1, gk::Color::White);
	m_textInput.setPadding(5, 6);
	m_textInput.setScale(Config::guiScale, Config::guiScale);

	m_createButton.setText(originalName.empty() ? "Create" : "Rename");
	m_createButton.setScale(Config::guiScale, Config::guiScale);
	m_createButton.setCallback([this, titleScreen, originalName](TextButton &) {
		std::string worldName = m_textInput.string();
		if (!fs::exists("saves/" + worldName + ".dat")) {
			if (gk::regexMatch(worldName, "^[A-Za-z0-9_]+$") && worldName[0] != '_') {
				if (!originalName.empty()) {
					fs::copy_file("saves/" + originalName + ".dat", "saves/" + worldName  + ".dat");
					fs::remove("saves/" + originalName + ".dat");

					m_stateStack->pop();

					// FIXME: This is needed because there's currently no way to refresh WorldSelectionState
					m_stateStack->pop(); // WorldSelectionState
					m_stateStack->push<WorldSelectionState>(titleScreen);
				}
				else
					m_stateStack->pop();

				if (originalName.empty())
					titleScreen->startSingleplayer(true, worldName);
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

	m_errorText.setColor(gk::Color::Red);
	m_errorText.setScale(Config::guiScale, Config::guiScale);

	updateWidgetPosition();
}

void WorldCreationState::onEvent(const sf::Event &event) {
	InterfaceState::onEvent(event);

	if (!m_stateStack->empty() && &m_stateStack->top() == this) {
		m_textInput.onEvent(event);

		m_createButton.onEvent(event);
		m_cancelButton.onEvent(event);
	}
}

void WorldCreationState::update() {
}

void WorldCreationState::updateWidgetPosition() {
	m_textInput.setPosition(
		Config::screenWidth / 2.0f - m_textInput.getBackgroundSize().x * Config::guiScale / 2.0f,
		Config::screenHeight / 2.0f - m_textInput.getBackgroundSize().y * Config::guiScale / 2.0f
	);

	m_createButton.setPosition(Config::screenWidth / 2.0f - m_createButton.getGlobalBounds().sizeX / 2, Config::screenHeight - 340);
	m_cancelButton.setPosition(Config::screenWidth / 2.0f - m_cancelButton.getGlobalBounds().sizeX / 2, Config::screenHeight - 261);

	m_errorText.setPosition(
		Config::screenWidth / 2.0f - m_errorText.getSize().x * Config::guiScale / 2.0f,
		Config::screenHeight / 2.0f - 30 * Config::guiScale
	);
}

void WorldCreationState::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	if (m_parent)
		target.draw(*m_parent, states);

	if (&m_stateStack->top() == this) {
		prepareDraw(target, states);

		target.draw(m_textInput, states);

		target.draw(m_createButton, states);
		target.draw(m_cancelButton, states);

		if (!m_errorText.string().empty())
			target.draw(m_errorText, states);
	}
}

