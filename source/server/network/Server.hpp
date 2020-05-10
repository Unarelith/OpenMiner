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
#ifndef SERVER_HPP_
#define SERVER_HPP_

#include <functional>
#include <unordered_map>

#include <SFML/Network/SocketSelector.hpp>
#include <SFML/Network/TcpListener.hpp>

#include "Network.hpp"
#include "ServerInfo.hpp"

class Server {
	using ConnectionCallback = std::function<void(ClientInfo &)>;
	using CommandCallback = std::function<void(ClientInfo &, Network::Packet &packet)>;

	public:
		void init(u16 port = 4242);

		void handleGameEvents();

		void sendToAllClients(Network::Packet &packet) const;

		bool isRunning() const { return m_isRunning; }

		u16 port() const { return m_port; }

		const ServerInfo &info() const { return m_info; }

		void setRunning(bool isRunning) { m_isRunning = isRunning; }
		void setSingleplayer(bool isSingleplayer) { m_isSingleplayer = isSingleplayer; }

		void setConnectionCallback(const ConnectionCallback &callback) { m_connectionCallback = callback; }
		void setCommandCallback(Network::Command command, const CommandCallback &callback) { m_commands[command] = callback; }

	private:
		void handleNewConnections();
		void handleClientMessages();

		void disconnectClient(ClientInfo &client);

		bool m_isRunning = false;
		bool m_isSingleplayer = false;

		u16 m_port = 0;

		ServerInfo m_info;

		sf::TcpListener m_tcpListener;
		sf::SocketSelector m_selector;

		ConnectionCallback m_connectionCallback;

		std::unordered_map<Network::Command, CommandCallback> m_commands;
};

#endif // SERVER_HPP_
