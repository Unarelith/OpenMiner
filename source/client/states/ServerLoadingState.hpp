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
#ifndef SERVERLOADINGSTATE_HPP_
#define SERVERLOADINGSTATE_HPP_

#include "Image.hpp"
#include "InterfaceState.hpp"
#include "Text.hpp"

class GameState;
struct ServerOnlineEvent;

class ServerLoadingState : public InterfaceState {
	public:
		ServerLoadingState(GameState &game, bool showLoadingState, const std::string &host, u16 port, gk::ApplicationState *parent = nullptr);

		void init() override;

		void update() override;

		void setUsername(const std::string &username) { m_username = username; }

	private:
		void onServerOnlineEvent(const ServerOnlineEvent &event);

		void updateWidgetPosition() override;

		void draw(RenderTarget &target, RenderStates states) const override;

		Image m_background{"texture-menu_background"};
		RectangleShape m_filter;

		Text m_text;

		GameState &m_game;

		bool m_showLoadingState;
		mutable bool m_hasBeenRendered = false;

		std::string m_username;

		bool m_isServerOnline = false;
		bool m_isConnected = false;

		std::string m_host;
		u16 m_port = 4242;
};

#endif // SERVERLOADINGSTATE_HPP_
