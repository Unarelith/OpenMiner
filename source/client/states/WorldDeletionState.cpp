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

#include <filesystem.hpp>

#include "Config.hpp"
#include "TitleScreenState.hpp"
#include "WorldDeletionState.hpp"
#include "WorldSelectionState.hpp"

namespace fs = ghc::filesystem;

WorldDeletionState::WorldDeletionState(const std::string &worldName, TitleScreenState *titleScreen) : InterfaceState(titleScreen) {
	m_text.setString("Are you sure you want to delete '" + worldName + "'?");
	m_text.setColor(gk::Color::White);
	m_text.updateVertexBuffer();
	m_text.setScale(Config::guiScale * 2, Config::guiScale * 2);

	m_confirmButton.setText("Yes");
	m_confirmButton.setScale(Config::guiScale, Config::guiScale);
	m_confirmButton.setCallback([this, worldName, titleScreen](TextButton &) {
		std::string saveFilepath = "saves/" + worldName + ".dat";
		if (fs::exists(saveFilepath))
			fs::remove(saveFilepath);
		m_stateStack->pop(); // WorldDeletionState
		m_stateStack->pop(); // WorldMenuState

		// FIXME: This is needed because there's currently no way to refresh WorldSelectionState
		m_stateStack->pop(); // WorldSelectionState
		m_stateStack->push<WorldSelectionState>(titleScreen);
	});

	m_cancelButton.setText("No");
	m_cancelButton.setScale(Config::guiScale, Config::guiScale);
	m_cancelButton.setCallback([this](TextButton &) {
		m_stateStack->pop();
	});

	updateWidgetPosition();
}

void WorldDeletionState::onEvent(const sf::Event &event) {
	InterfaceState::onEvent(event);

	if (event.type == sf::Event::Resized) {
		updateWidgetPosition();
	}

	if (&m_stateStack->top() == this) {
		m_confirmButton.onEvent(event);
		m_cancelButton.onEvent(event);
	}
}

void WorldDeletionState::update() {
}

void WorldDeletionState::updateWidgetPosition() {
	m_text.setPosition(Config::screenWidth  / 2 - m_text.getSize().x * Config::guiScale * 2 / 2,
	                   Config::screenHeight / 2 - m_text.getSize().y * Config::guiScale * 2 / 2);

	m_confirmButton.setPosition(Config::screenWidth / 2.0f - m_confirmButton.getGlobalBounds().sizeX / 2, Config::screenHeight - 340);
	m_cancelButton.setPosition(Config::screenWidth / 2.0f - m_cancelButton.getGlobalBounds().sizeX / 2, Config::screenHeight - 261);
}

void WorldDeletionState::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	if (m_parent)
		target.draw(*m_parent, states);

	if (&m_stateStack->top() == this) {
		prepareDraw(target, states);

		target.draw(m_text, states);

		target.draw(m_confirmButton, states);
		target.draw(m_cancelButton, states);
	}
}

