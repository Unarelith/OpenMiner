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
#ifndef INPUTSYSTEM_HPP_
#define INPUTSYSTEM_HPP_

#include <gk/core/ApplicationStateStack.hpp>
#include <gk/gl/Camera.hpp>
#include <gk/gl/Drawable.hpp>
#include <gk/gl/Shader.hpp>

#include "Config.hpp"
#include "Framebuffer.hpp"
#include "HUD.hpp"
#include "PlayerBox.hpp"

class ClientWorld;
class Skybox;

class InputSystem {
	public:
		InputSystem(gk::Camera &camera, ClientWorld &world, Skybox &skybox, HUD &hud, ClientPlayer &player, Client &client, ClientCommandHandler &clientCommandHandler)
			: m_camera(camera), m_world(world), m_skybox(skybox), m_hud(hud), m_player(player), m_client(client), m_clientCommandHandler(clientCommandHandler) {}

		void onEvent(const SDL_Event &event, gk::ApplicationStateStack *stateStack, gk::ApplicationState *currentState);

		void update(gk::ApplicationStateStack *stateStack, gk::ApplicationState *currentState);

	private:
		// Event actions
		void rotateCamera(const SDL_Event &event);
		void pauseGame(gk::ApplicationStateStack *stateStack, gk::ApplicationState *currentState);
		void grabCursor();
		void ungrabCursor();
		void openChat(bool addSlash, gk::ApplicationStateStack *stateStack, gk::ApplicationState *currentState);
		void takeScreenshot();
		void sendKeyPressToServer(const SDL_Event &event);

		// Update process
		void setupInputs();
		void updateWorld(gk::ApplicationStateStack *stateStack, gk::ApplicationState *currentState);
		void updateScene(gk::ApplicationStateStack *stateStack, gk::ApplicationState *currentState);
		void updateClient();

		gk::Camera &m_camera;
		ClientWorld &m_world;
		Skybox &m_skybox;
		HUD &m_hud;
		ClientPlayer &m_player;
		Client &m_client;
		ClientCommandHandler &m_clientCommandHandler;

		bool m_areModKeysLoaded = false;
};

#endif // INPUTSYSTEM_HPP_
