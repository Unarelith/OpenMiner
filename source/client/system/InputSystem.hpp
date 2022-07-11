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
#include <gk/gl/Drawable.hpp>
#include <gk/gl/Shader.hpp>

#include "Config.hpp"
#include "Framebuffer.hpp"
#include "PlayerBox.hpp"

class Client;
class ClientCommandHandler;
class ClientPlayer;
class ClientWorld;
class HUD;
class MessageBus;
class Skybox;

class InputSystem {
	public:
		InputSystem(HUD &hud, ClientCommandHandler &clientCommandHandler, MessageBus &messageBus)
			: m_hud(hud), m_clientCommandHandler(clientCommandHandler), m_messageBus(messageBus) {}

		void setStateInfo(gk::ApplicationStateStack *stateStack, gk::ApplicationState *currentState) {
			m_stateStack = stateStack;
			m_currentState = currentState;
		}

		void onEvent(const SDL_Event &event);

		void update();

	private:
		void setupInputs();

		HUD &m_hud; // forward events (will be a dedicated GUI system later)
		ClientCommandHandler &m_clientCommandHandler; // forward key press events to server (there will probably be a network system later)

		MessageBus &m_messageBus;

		// Required for some checks
		gk::ApplicationStateStack *m_stateStack;
		gk::ApplicationState *m_currentState;

		// Required for the initial loading of mod-defined key bindings
		bool m_areModKeysLoaded = false;
};

#endif // INPUTSYSTEM_HPP_
