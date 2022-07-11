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
#include <gk/core/Mouse.hpp>
#include <gk/resource/ResourceHandler.hpp>

#include "Events.hpp"
#include "GameState.hpp"
#include "Registry.hpp"
#include "TextureAtlas.hpp"

GameState::GameState()
	: m_textureAtlas(gk::ResourceHandler::getInstance().get<TextureAtlas>("atlas-blocks"))
{
	Registry::setInstance(m_registry);

	m_renderingSystem.init();

	m_clientCommandHandler.setupCallbacks();

	m_world.setClient(m_clientCommandHandler);
	m_world.setCamera(m_player.camera());
}

void GameState::init() {
	m_world.setEventHandler(*m_eventHandler);

	m_eventHandler->addListener<GuiScaleChangedEvent>(&GameState::onGuiScaleChanged, this);

	m_eventHandler->addListener<ChunkCreatedEvent>(&Minimap::onChunkCreatedEvent, &m_hud.minimap());
	m_eventHandler->addListener<ChunkRemovedEvent>(&Minimap::onChunkRemovedEvent, &m_hud.minimap());

	m_inputSystem.setStateInfo(m_stateStack, this);
	m_gameplaySystem.setStateInfo(m_stateStack, this);
}

void GameState::onStateInactive() {
	m_hud.pause();
}

void GameState::connect(const std::string &host, u16 port, const std::string &username) {
	m_player.setName(username.empty() ? "Player" : username);
	m_client.connect(host, port, m_player);
	m_player.setClientID(m_client.id());
	m_client.addPlayer(m_player);

	gk::Mouse::setCursorVisible(false);
	gk::Mouse::setCursorGrabbed(true);
}

void GameState::onEvent(const SDL_Event &event) {
	if (event.type == SDL_QUIT) {
		m_client.disconnect();

		m_stateStack->clear();
	}

	if (!m_stateStack->empty() && &m_stateStack->top() == this)
		m_inputSystem.onEvent(event);
}

void GameState::update() {
	// Update systems
	m_inputSystem.update();
	m_gameplaySystem.update();
	m_renderingSystem.update();

	// Process events
	m_messageBus.update();
}

void GameState::onGuiScaleChanged(const GuiScaleChangedEvent &event) {
	m_hud.onGuiScaleChanged(event);
}

void GameState::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	target.draw(m_renderingSystem, states);
}

