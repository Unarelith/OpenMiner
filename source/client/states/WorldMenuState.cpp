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
#include "WorldMenuState.hpp"
#include "WorldCreationState.hpp"
#include "WorldSelectionState.hpp"

namespace fs = ghc::filesystem;

WorldMenuState::WorldMenuState(const std::string &worldName, TitleScreenState *titleScreen) : InterfaceState(titleScreen) {
	m_menuWidget.setScale(Config::guiScale, Config::guiScale);

	m_menuWidget.addButton("Play", [worldName, titleScreen](TextButton &) {
		titleScreen->startSingleplayer(true, worldName);
	});

	m_menuWidget.addButton("Rename", [this, titleScreen, worldName](TextButton &) {
		m_stateStack->push<WorldCreationState>(titleScreen, worldName);
	});

	m_menuWidget.addButton("Delete", [this, worldName, titleScreen](TextButton &) {
		m_stateStack->push<WorldDeletionState>(worldName, titleScreen);
	});

	m_cancelButton.setText("Cancel");
	m_cancelButton.setScale(Config::guiScale, Config::guiScale);
	m_cancelButton.setCallback([this](TextButton &) {
		m_stateStack->pop();
	});

	updateWidgetPosition();
}

void WorldMenuState::onEvent(const sf::Event &event) {
	InterfaceState::onEvent(event);

	if (event.type == sf::Event::Resized && !m_stateStack->empty() && &m_stateStack->top() != this) {
		m_menuWidget.onEvent(event);
	}

	if (!m_stateStack->empty() && &m_stateStack->top() == this) {
		m_menuWidget.onEvent(event);
		m_cancelButton.onEvent(event);
	}
}

void WorldMenuState::update() {
}

void WorldMenuState::updateWidgetPosition() {
	m_cancelButton.setPosition(Config::screenWidth / 2.0f - m_cancelButton.getGlobalBounds().sizeX / 2.0f, Config::screenHeight * 0.85);
}

void WorldMenuState::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	if (m_parent)
		target.draw(*m_parent, states);

	if (&m_stateStack->top() == this) {
		prepareDraw(target, states);

		target.draw(m_menuWidget, states);
		target.draw(m_cancelButton, states);
	}
}
