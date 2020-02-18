/*
 * =====================================================================================
 *
 *  OpenMiner
 *  Copyright (C) 2018-2020 Unarelith, Quentin Bazin <openminer@unarelith.net>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *
 * =====================================================================================
 */
#ifndef CLIENTAPPLICATION_HPP_
#define CLIENTAPPLICATION_HPP_

#include <gk/core/CoreApplication.hpp>
#include <gk/core/input/KeyboardHandler.hpp>

#include "Registry.hpp"

class ClientApplication : public gk::CoreApplication {
	public:
		ClientApplication(int argc, char **argv);

		void init() override;

	private:
		void handleEvents() override;

		void onEvent(const SDL_Event &event) override;

		void initOpenGL();

		gk::KeyboardHandler m_keyboardHandler;

		Registry m_registry;

		std::string m_host = "localhost";
		int m_port = 4242;
};

#endif // CLIENTAPPLICATION_HPP_
