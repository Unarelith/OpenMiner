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
#ifndef CONNECTIONERRORSTATE_HPP_
#define CONNECTIONERRORSTATE_HPP_

#include "InterfaceState.hpp"
#include "MenuWidget.hpp"

class ConnectionErrorState : public InterfaceState {
	public:
		ConnectionErrorState(const std::string &error, const std::string &host, u16 port, gk::ApplicationState *parent = nullptr);

		void onEvent(const SDL_Event &event) override;

	private:
		void updateWidgetPosition() override;

		void draw(RenderTarget &target, RenderStates states) const override;

		std::string m_host;
		u16 m_port;

		gk::Image m_background{"texture-menu_background"};
		gk::RectangleShape m_filter;

		Text m_text;

		MenuWidget m_menuWidget{1, 2};
};

#endif // CONNECTIONERRORSTATE_HPP_
