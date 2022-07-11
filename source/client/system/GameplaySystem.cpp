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
#include <gk/core/Mouse.hpp>

#include "Client.hpp"
#include "ClientCommandHandler.hpp"
#include "ClientPlayer.hpp"
#include "Config.hpp"
#include "GameplaySystem.hpp"
#include "HUD.hpp"
#include "MessageBus.hpp"
#include "RenderingSystem.hpp"
#include "Skybox.hpp"

#include "ChatState.hpp"
#include "PauseMenuState.hpp"

GameplaySystem::GameplaySystem(ClientPlayer &player, Client &client, ClientCommandHandler &clientCommandHandler, HUD &hud, MessageBus &messageBus, Skybox &skybox, ClientWorld &world, std::unordered_map<u16, PlayerBox> &playerBoxes)
	: m_player(player), m_client(client), m_clientCommandHandler(clientCommandHandler), m_hud(hud), m_messageBus(messageBus), m_skybox(skybox), m_world(world), m_playerBoxes(playerBoxes)
{
	m_messageBus.subscribe<GameplayEvent::RotateCamera, &GameplaySystem::onRotateCamera>(*this);
	m_messageBus.subscribe<GameplayEvent::PauseGame, &GameplaySystem::onPauseGame>(*this);
	m_messageBus.subscribe<GameplayEvent::OpenChat, &GameplaySystem::onOpenChat>(*this);
	m_messageBus.subscribe<GameplayEvent::TakeScreenshot, &GameplaySystem::onTakeScreenshot>(*this);
	m_messageBus.subscribe<GameplayEvent::GameUpdate, &GameplaySystem::onGameUpdate>(*this);
}

void GameplaySystem::update() {
	RenderingEvent::DrawObjects event;
	{
		event.inFramebuffer = true;
		event.objects.emplace_back(&m_skybox);
		event.objects.emplace_back(&m_world);

		for (auto &it : m_playerBoxes)
			if (it.second.dimension() == m_player.dimension())
				event.objects.emplace_back(&it.second);

		event.objects.emplace_back(&m_hud.blockCursor());
	}
	m_messageBus.publish(event);

	RenderingEvent::DrawObjects event2;
	{
		event2.inFramebuffer = false;
		event2.objects.emplace_back(&m_hud);
	}
	m_messageBus.publish(event2);
}

//==============================================================================
// Event callbacks
//==============================================================================

void GameplaySystem::onRotateCamera(const GameplayEvent::RotateCamera &event) {
	if(Config::screenWidth / 2 != event.x || Config::screenHeight / 2 != event.y) {
		m_player.turnH((float)event.xrel * -0.01f * Config::mouseSensitivity);
		m_player.turnViewV((float)event.yrel * -0.01f * Config::mouseSensitivity);

		gk::Mouse::resetToWindowCenter();
	}
}

void GameplaySystem::onPauseGame(const GameplayEvent::PauseGame &event) {
	event.stateStack->push<PauseMenuState>(m_client, event.currentState);
}

void GameplaySystem::onOpenChat(const GameplayEvent::OpenChat &event) {
	event.stateStack->push<ChatState>(m_clientCommandHandler, m_hud.chat(), event.addSlash, event.currentState);
}

void GameplaySystem::onTakeScreenshot() {
	std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

	char filename[100];
	std::strftime(filename, sizeof(filename), "Screenshot-%Y-%m-%d-%H-%M-%S.png", std::localtime(&now));

	bool isScreenshotSaved = gk::Window::saveScreenshot(0, 0, Config::screenWidth, Config::screenHeight, filename);
	if (isScreenshotSaved)
		m_hud.chat().addChatMessage(0, "Screenshot saved: " + std::string(filename));
	else
		m_hud.chat().addChatMessage(0, "Failed to save screenshot");
}

void GameplaySystem::onGameUpdate(const GameplayEvent::GameUpdate &event) {
	// Update world
	m_world.checkPlayerChunk(m_player.x(), m_player.y(), m_player.z());
	m_world.update(event.allowWorldReload);

	// Update player
	if (event.processPlayerInputs) {
		m_player.processInputs();
	}
	m_player.updatePosition(m_world);

	// Update HUD
	m_hud.update();

	// Update client
	if (event.sendPlayerPosRotUpdate) {
		m_clientCommandHandler.sendPlayerPosUpdate();
		m_clientCommandHandler.sendPlayerRotUpdate();
	}
	m_client.update();
}

