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
	m_background.setScale(Config::guiScale * 2, Config::guiScale * 2);

	m_filter.setFillColor(gk::Color::fromRGBA32(0, 0, 0, 176));

	m_text1.setString("Are you sure you want to delete this world?");
	m_text1.updateVertexBuffer();
	m_text1.setScale(Config::guiScale, Config::guiScale);

	m_text2.setString("'" + worldName + "' will be lost forever! (A long time!)");
	m_text2.updateVertexBuffer();
	m_text2.setScale(Config::guiScale, Config::guiScale);

	m_menuWidget.setScale(Config::guiScale, Config::guiScale);
	m_menuWidget.addButton("Yes", [this, worldName, titleScreen](TextButton &) {
		std::string saveFilepath = "saves/" + worldName + ".dat";
		if (fs::exists(saveFilepath))
			fs::remove(saveFilepath);
		m_stateStack->pop(); // WorldDeletionState

		// FIXME: This is needed because there's currently no way to refresh WorldSelectionState
		m_stateStack->pop(); // WorldSelectionState
		m_stateStack->push<WorldSelectionState>(titleScreen);
	}, 150);

	m_menuWidget.addButton("No", [this](TextButton &) {
		m_stateStack->pop();
	}, 150);

	updateWidgetPosition();
}

void WorldDeletionState::onEvent(const SDL_Event &event) {
	InterfaceState::onEvent(event);

	if (&m_stateStack->top() == this) {
		m_menuWidget.onEvent(event);
	}
}

void WorldDeletionState::updateWidgetPosition() {
	m_background.setPosRect(0, 0,
		u16(Config::screenWidth / m_background.getScale().x),
		u16(Config::screenHeight / m_background.getScale().y)
	);

	m_background.setClipRect(0, 0,
		u16(Config::screenWidth / m_background.getScale().x),
		u16(Config::screenHeight / m_background.getScale().y)
	);

	m_filter.setSize(Config::screenWidth, Config::screenHeight);

	m_text1.setPosition(Config::screenWidth  / 2.f - float(m_text1.getSize().x * Config::guiScale) / 2.f,
	                    Config::screenHeight / 2.f - 30.f * Config::guiScale);

	m_text2.setPosition(Config::screenWidth  / 2.f - float(m_text2.getSize().x * Config::guiScale) / 2.f,
	                    Config::screenHeight / 2.f - float(m_text2.getSize().y * Config::guiScale) / 2.f);

	m_menuWidget.setPosition(
		Config::screenWidth / 2.0f - m_menuWidget.getGlobalBounds().sizeX / 2,
		Config::screenHeight - 110.f * Config::guiScale
	);
}

void WorldDeletionState::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	if (m_parent)
		target.draw(*m_parent, states);

	if (&m_stateStack->top() == this) {
		prepareDraw(target, states);

		target.draw(m_background, states);
		target.draw(m_filter, states);

		target.draw(m_text1, states);
		target.draw(m_text2, states);

		target.draw(m_menuWidget, states);
	}
}

