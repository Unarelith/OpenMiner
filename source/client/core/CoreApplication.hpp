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
#ifndef COREAPPLICATION_HPP_
#define COREAPPLICATION_HPP_

#include "common/core/ApplicationStateStack.hpp"
#include "common/core/ArgumentParser.hpp"
#include "common/core/EventHandler.hpp"
#include "common/core/GameClock.hpp"
#include "common/resource/ResourceHandler.hpp"

#include "client/core/SDLLoader.hpp"
#include "client/core/Window.hpp"

class CoreApplication {
	public:
		CoreApplication(int argc, char **argv);
		virtual ~CoreApplication() = default;

		int run(bool isProtected = true);

		static bool hasBeenInterrupted;

	protected:
		virtual bool init();

		void exit();

		void createWindow(u16 width, u16 height, const std::string &title);

		virtual void onEvent(const SDL_Event &event);

		virtual void onExit() {}

		virtual void handleEvents();

		virtual void mainLoop() = 0;

		SDLLoader m_sdlLoader;
		bool m_loadSDL = true;

		ApplicationStateStack m_stateStack;

		GameClock m_clock;

		ResourceHandler m_resourceHandler;

		Window m_window;

		RenderStates m_renderStates = RenderStates::Default;

		ArgumentParser m_argumentParser;

		EventHandler m_eventHandler;

		LoggerHandler m_loggerHandler;

		ApplicationState *m_currentState = nullptr;
};

#endif // COREAPPLICATION_HPP_
