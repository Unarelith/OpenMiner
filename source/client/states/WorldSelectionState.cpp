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
#include <gk/core/Debug.hpp>

#include <filesystem.hpp>

#include "Config.hpp"
#include "TitleScreenState.hpp"
#include "WorldCreationState.hpp"
#include "WorldDeletionState.hpp"
#include "WorldSelectionState.hpp"

namespace fs = ghc::filesystem;

WorldSelectionState::WorldSelectionState(TitleScreenState *titleScreen)
	: InterfaceState(titleScreen), m_titleScreen(titleScreen)
{
	m_background.setScale(Config::guiScale * 2, Config::guiScale * 2);

	m_filter1.setFillColor(gk::Color(0, 0, 0, 192));
	m_filter2.setFillColor(gk::Color(0, 0, 0, 120));

	m_title.setScale(Config::guiScale, Config::guiScale);
	m_title.setString("Select World");
	m_title.updateVertexBuffer();

	m_worldList.setScale(Config::guiScale, Config::guiScale);

	m_menuWidget1.setScale(Config::guiScale, Config::guiScale);
	m_menuWidget1.setHorizontalSpacing(8);
	m_menuWidget1.addButton("Play Selected World", [this](TextButton &) {
		const ScrollableListElement *element = m_worldList.selectedElement();
		if (element)
			m_titleScreen->startSingleplayer(true, element->line1());
	}, 150).setEnabled(false);
	m_menuWidget1.addButton("Create New World", [this](TextButton &) {
		m_stateStack->push<WorldCreationState>(m_titleScreen);
	}, 150);

	m_menuWidget2.setScale(Config::guiScale, Config::guiScale);
	m_menuWidget2.setHorizontalSpacing(6);
	m_menuWidget2.addButton("Edit", [this](TextButton &) {
		const ScrollableListElement *element = m_worldList.selectedElement();
		if (element)
			m_stateStack->push<WorldCreationState>(m_titleScreen, element->line1());
	}, 72).setEnabled(false);
	m_menuWidget2.addButton("Delete", [this](TextButton &) {
		const ScrollableListElement *element = m_worldList.selectedElement();
		if (element)
			m_stateStack->push<WorldDeletionState>(element->line1(), m_titleScreen);
	}, 72).setEnabled(false);

	m_menuWidget3.setScale(Config::guiScale, Config::guiScale);
	m_menuWidget3.setHorizontalSpacing(6);
	m_menuWidget3.addButton("Re-Create", [](TextButton &) {
	}, 72).setEnabled(false);
	m_menuWidget3.addButton("Cancel", [this](TextButton &) {
		m_stateStack->pop();
	}, 72);

	updateWidgetPosition();
	loadSaveList();
}

void WorldSelectionState::onEvent(const SDL_Event &event) {
	InterfaceState::onEvent(event);

	if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED
	&& !m_stateStack->empty() && &m_stateStack->top() != this) {
		m_worldList.onEvent(event);
	}

	if (!m_stateStack->empty() && &m_stateStack->top() == this) {
		m_worldList.onEvent(event);

		if (m_worldList.selectedElement()) {
			m_menuWidget1.setButtonEnabled("Play Selected World", true);
			m_menuWidget2.setButtonEnabled("Edit", true);
			m_menuWidget2.setButtonEnabled("Delete", true);
		}
		else {
			m_menuWidget1.setButtonEnabled("Play Selected World", false);
			m_menuWidget2.setButtonEnabled("Edit", false);
			m_menuWidget2.setButtonEnabled("Delete", false);
		}

		m_menuWidget1.onEvent(event);
		m_menuWidget2.onEvent(event);
		m_menuWidget3.onEvent(event);
	}
}

void WorldSelectionState::update() {
}

void WorldSelectionState::updateWidgetPosition() {
	m_background.setPosRect(0, 0, Config::screenWidth / m_background.getScale().x, Config::screenHeight / m_background.getScale().y);
	m_background.setClipRect(0, 0, Config::screenWidth / m_background.getScale().x, Config::screenHeight / m_background.getScale().y);

	m_filter1.setSize(Config::screenWidth, Config::screenHeight);

	const int topBorderSize = 25 * Config::guiScale;
	const int bottomBorderSize = 50 * Config::guiScale;
	m_filter2.setSize(Config::screenWidth, Config::screenHeight - topBorderSize - bottomBorderSize);
	m_filter2.setPosition(0, topBorderSize);

	m_title.setPosition(
		Config::screenWidth / 2.0f - m_title.getSize().x * Config::guiScale / 2.0f,
		topBorderSize / 2.0f - m_title.getSize().y * Config::guiScale / 2.0f
	);

	m_worldList.setPosition(
		Config::screenWidth / 2.0f - m_worldList.getGlobalBounds().sizeX / 2.0f,
		topBorderSize + 2.0f * Config::guiScale
	);

	m_menuWidget1.setPosition(
		Config::screenWidth / 2.0f - m_menuWidget1.getGlobalBounds().sizeX / 2.0f,
		Config::screenHeight - bottomBorderSize / 2.0f - m_menuWidget1.getGlobalBounds().sizeY - 2.0f
	);

	m_menuWidget2.setPosition(
		Config::screenWidth / 2.0f - m_menuWidget2.getGlobalBounds().sizeX - 4 * Config::guiScale,
		Config::screenHeight - bottomBorderSize / 2.0f + 2 * Config::guiScale
	);

	m_menuWidget3.setPosition(
		Config::screenWidth / 2.0f + 4 * Config::guiScale,
		Config::screenHeight - bottomBorderSize / 2.0f + 2 * Config::guiScale
	);
}

void WorldSelectionState::loadSaveList() {
	if (fs::is_directory("saves")) {
		std::vector<fs::directory_entry> files;

		fs::path basePath = fs::current_path();
		fs::directory_iterator dir("saves/");
		for (const auto &entry : dir) {
			if (entry.is_regular_file() && entry.path().filename().generic_string()[0] != '_') {
				files.emplace_back(entry);
			}
		}

		std::sort(files.begin(), files.end());

		for (auto &entry : files) {
			std::string filename = entry.path().filename();
			if (filename.substr(filename.find_last_of('.')) == ".dat") {
				std::string saveFile = filename.substr(0, filename.find_last_of('.'));
				std::string filesize = std::to_string(entry.file_size() / 1000.f / 1000.f);
				m_worldList.addElement(saveFile, filesize.substr(0, filesize.find_first_of('.') + 3) + " MB", "");
			}
		}
	}
}

void WorldSelectionState::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	if (&m_stateStack->top() == this) {
		prepareDraw(target, states);

		target.draw(m_background, states);
		target.draw(m_filter1, states);
		target.draw(m_filter2, states);

		target.draw(m_title, states);

		target.draw(m_worldList, states);
		target.draw(m_menuWidget1, states);
		target.draw(m_menuWidget2, states);
		target.draw(m_menuWidget3, states);
	}
}

