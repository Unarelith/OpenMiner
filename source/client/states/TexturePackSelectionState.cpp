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
#include <gk/resource/ResourceHandler.hpp>

#include <filesystem.hpp>

#include "Config.hpp"
#include "GameConfig.hpp"
#include "TextureAtlas.hpp"
#include "TexturePackSelectionState.hpp"
#include "World.hpp"

namespace fs = ghc::filesystem;

TexturePackSelectionState::TexturePackSelectionState(DrawableState *parent) : InterfaceState(parent) {
	m_background.setScale(Config::guiScale * 2.f, Config::guiScale * 2.f);

	m_filter1.setFillColor(gk::Color::fromRGBA32(0, 0, 0, 192));
	m_filter2.setFillColor(gk::Color::fromRGBA32(0, 0, 0, 120));

	m_title.setScale(Config::guiScale, Config::guiScale);
	m_title.setString("Select Texture Pack");
	m_title.updateVertexBuffer();

	m_texturePackList.setScale(Config::guiScale, Config::guiScale);

	m_menuWidget.setScale(Config::guiScale, Config::guiScale);
	m_menuWidget.setHorizontalSpacing(8);
	m_menuWidget.addButton("Select Texture Pack", [this](TextButton &) {
		const ScrollableListElement *element = m_texturePackList.selectedElement();
		if (element) {
			std::string texturePack = (element->id()) ? element->line1() : "";
			if (Config::texturePack != texturePack) {
				Config::texturePack = texturePack;

				if (GameConfig::isGameRunning) {
					auto &atlas = gk::ResourceHandler::getInstance().get<TextureAtlas>("atlas-blocks");
					atlas.clear();
					atlas.loadFromRegistry(Config::texturePack);

					World::isReloadRequested = true;
				}
			}
		}

		m_stateStack->pop();
	}, 150);
	m_menuWidget.addButton("Cancel", [this](TextButton &) {
		m_stateStack->pop();
	}, 150);

	updateWidgetPosition();
	loadTexturePackList();
}

void TexturePackSelectionState::onEvent(const SDL_Event &event) {
	InterfaceState::onEvent(event);

	if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
		m_stateStack->pop();

	if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED
	&& !m_stateStack->empty() && &m_stateStack->top() != this) {
		m_texturePackList.onEvent(event);
	}

	if (!m_stateStack->empty() && &m_stateStack->top() == this) {
		m_texturePackList.onEvent(event);

		m_menuWidget.onEvent(event);
	}
}

void TexturePackSelectionState::updateWidgetPosition() {
	m_background.setPosRect(0, 0,
		u16(Config::screenWidth / m_background.getScale().x),
		u16(Config::screenHeight / m_background.getScale().y)
	);

	m_background.setClipRect(0, 0,
		u16(Config::screenWidth / m_background.getScale().x),
		u16(Config::screenHeight / m_background.getScale().y)
	);

	m_filter1.setSize(Config::screenWidth, Config::screenHeight);

	const int topBorderSize = 25 * Config::guiScale;
	const int bottomBorderSize = 25 * Config::guiScale;
	m_filter2.setSize(Config::screenWidth, float(Config::screenHeight - topBorderSize - bottomBorderSize));
	m_filter2.setPosition(0, (float)topBorderSize);

	m_title.setPosition(
		Config::screenWidth / 2.0f - float(m_title.getSize().x * Config::guiScale) / 2.0f,
		(float)topBorderSize / 2.0f - float(m_title.getSize().y * Config::guiScale) / 2.0f
	);

	m_texturePackList.setPosition(
		Config::screenWidth / 2.0f - m_texturePackList.getGlobalBounds().sizeX / 2.0f,
		(float)topBorderSize + 2.0f * Config::guiScale
	);

	m_menuWidget.setPosition(
		Config::screenWidth / 2.0f - m_menuWidget.getGlobalBounds().sizeX / 2.0f,
		Config::screenHeight - (float)bottomBorderSize / 2.0f - m_menuWidget.getGlobalBounds().sizeY / 2.0f
	);
}

void TexturePackSelectionState::loadTexturePackList() {
	m_texturePackList.addElement("No Texture Pack", "Use default mod textures", "", Config::texturePack.empty());

	if (fs::is_directory("texturepacks")) {
		std::vector<fs::directory_entry> dirs;

		fs::directory_iterator dir("texturepacks/");
		for (const auto &entry : dir) {
			if (entry.is_directory()) {
				dirs.emplace_back(entry);
			}
		}

		std::sort(dirs.begin(), dirs.end());

		for (auto &entry : dirs) {
			std::string dirname = entry.path().filename();
			std::string textures;
			if (fs::exists(entry.path().string() + "/blocks")) {
				textures += "Blocks";
			}
			if (fs::exists(entry.path().string() + "/items")) {
				if (!textures.empty()) textures += ", ";
				textures += "Items";
			}
			if (fs::exists(entry.path().string() + "/gui")) {
				if (!textures.empty()) textures += ", ";
				textures += "GUI";
			}

			bool isSelected = Config::texturePack == dirname;
			m_texturePackList.addElement(dirname, textures, "", isSelected);
		}
	}
}

void TexturePackSelectionState::draw(RenderTarget &target, RenderStates states) const {
	if (&m_stateStack->top() == this) {
		prepareDraw(target, states);

		target.draw(m_background, states);
		target.draw(m_filter1, states);
		target.draw(m_filter2, states);

		target.draw(m_title, states);
		target.draw(m_texturePackList, states);
		target.draw(m_menuWidget, states);
	}
}

