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
#ifndef GAMEPLAYSYSTEM_HPP_
#define GAMEPLAYSYSTEM_HPP_

#include <gk/core/ApplicationStateStack.hpp>

class Client;
class ClientCommandHandler;
class ClientPlayer;
class ClientWorld;
class HUD;
class MessageBus;
class PlayerBox;
class Skybox;

namespace GameplayEvent {
	struct RotateCamera {
		int x, y;
		int xrel, yrel;
	};

	struct PauseGame {
		gk::ApplicationStateStack *stateStack;
		gk::ApplicationState *currentState;
	};

	struct OpenChat {
		bool addSlash;

		gk::ApplicationStateStack *stateStack;
		gk::ApplicationState *currentState;
	};

	struct TakeScreenshot {};

	struct GameUpdate {
		bool allowWorldReload;
		bool processPlayerInputs;
		bool sendPlayerPosRotUpdate;
	};
}

class GameplaySystem {
	public:
		GameplaySystem(ClientPlayer &player, Client &client, ClientCommandHandler &clientCommandHandler, HUD &hud, MessageBus &messageBus, Skybox &skybox, ClientWorld &world, std::unordered_map<u16, PlayerBox> &playerBoxes);

		void update();

		void setStateInfo(gk::ApplicationStateStack *stateStack, gk::ApplicationState *currentState) {
			m_stateStack = stateStack;
			m_currentState = currentState;
		}

	// private:
	public:
		void onRotateCamera(const GameplayEvent::RotateCamera &event);
		void onPauseGame(const GameplayEvent::PauseGame &event);
		void onOpenChat(const GameplayEvent::OpenChat &event);
		void onTakeScreenshot();
		void onGameUpdate(const GameplayEvent::GameUpdate &event);

	private:
		ClientPlayer &m_player; // dimension and camera
		Client &m_client; // pause menu
		ClientCommandHandler &m_clientCommandHandler; // chat
		HUD &m_hud; // draw events and chat

		MessageBus &m_messageBus;

		// Those three are only needed to send the draw events
		Skybox &m_skybox;
		ClientWorld &m_world;
		std::unordered_map<u16, PlayerBox> &m_playerBoxes;

		gk::ApplicationStateStack *m_stateStack;
		gk::ApplicationState *m_currentState;
};

#endif // GAMEPLAYSYSTEM_HPP_
