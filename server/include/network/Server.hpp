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
#ifndef SERVER_HPP_
#define SERVER_HPP_

#include <functional>
#include <unordered_map>

#include <SFML/Network/SocketSelector.hpp>
#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/UdpSocket.hpp>

#include "Network.hpp"
#include "ServerInfo.hpp"

class Server {
	using ConnectionCallback = std::function<void(Client&)>;
	using CommandCallback = std::function<void(Client &, sf::Packet &packet)>;

	public:
		void init(u16 port = 4242);

		void handleKeyState();

		void handleGameEvents();

		void sendToAllClients(sf::Packet &packet);

		bool isRunning() const { return m_isRunning; }

		ServerInfo &info() { return m_info; }

		sf::UdpSocket &udpSocket() { return m_udpSocket; }

		void setRunning(bool isRunning) { m_isRunning = isRunning; }

		void setConnectionCallback(const ConnectionCallback &callback) { m_connectionCallback = callback; }
		void setCommandCallback(Network::Command command, const CommandCallback &callback) { m_commands[command] = callback; }

	private:
		void handleNewConnections();
		void handleClientMessages();

		bool m_isRunning = true;

		ServerInfo m_info;

		sf::UdpSocket m_udpSocket;

		sf::TcpListener m_tcpListener;
		sf::SocketSelector m_selector;

		ConnectionCallback m_connectionCallback;

		std::unordered_map<Network::Command, CommandCallback> m_commands;
};

#endif // SERVER_HPP_
