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
#ifndef CLIENTAPPLICATION_HPP_
#define CLIENTAPPLICATION_HPP_

#include "ClientProfiler.hpp"
#include "CoreApplication.hpp"
#include "KeyboardHandler.hpp"

class ClientApplication : public CoreApplication {
	public:
		ClientApplication(int argc, char **argv);

		bool init() override;

	private:
		void handleEvents() override;

		void onEvent(const SDL_Event &event) override;
		void onExit() override;

		void mainLoop() override;

		static void initOpenGL();

		KeyboardHandler m_keyboardHandler;

		std::string m_host = "localhost";
		u16 m_port = 4242;

#ifdef OM_PROFILER_ENABLED
		ClientProfiler m_profiler;
#endif
};

#endif // CLIENTAPPLICATION_HPP_
