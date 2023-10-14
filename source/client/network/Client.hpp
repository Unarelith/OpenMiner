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
#ifndef CLIENT_HPP_
#define CLIENT_HPP_

#include <functional>
#include <unordered_map>

#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/TcpSocket.hpp>

#include <gk/core/Timer.hpp>

#include "Network.hpp"
#include "Player.hpp"

class ClientConnectException {
	public:
		ClientConnectException(const std::string &str)
			: m_str(str) {}

		const char *what() const noexcept {
			return m_str.c_str();
		}

	private:
		std::string m_str;
};

class Client {
	using CommandCallback = std::function<void(Network::Packet &packet)>;

	public:
		void connect(sf::IpAddress serverAddress, u16 serverPort, Player &player);
		void disconnect();

		void send(Network::Packet &packet);

		void update();

		void setCommandCallback(Network::Command command, const CommandCallback &callback) { m_commands[command] = callback; }

		bool isSingleplayer() const { return m_isSingleplayer; }
		void setSingleplayer(bool isSingleplayer) { m_isSingleplayer = isSingleplayer; }

		bool isConnected() const { return m_isConnected; }

		u16 id() const { return m_id; }

		sf::IpAddress serverAddress() const { return m_serverAddress; }
		u16 serverPort() const { return m_serverPort; }

		void addPlayer(Player &player) { m_players.emplace(player.clientID(), player); }
		void removePlayer(u16 clientID) { m_players.erase(clientID); }
		Player &getPlayer(u16 clientID) { return m_players.at(clientID); }

	private:
		bool m_isConnected = false;
		bool m_isSingleplayer = false;

		u16 m_id;

		sf::IpAddress m_serverAddress;
		u16 m_serverPort;

		std::unique_ptr<sf::TcpSocket> m_tcpSocket;

		gk::Timer m_keyUpdateTimer;

		std::unordered_map<Network::Command, CommandCallback> m_commands;

		std::string m_texturePack;
		std::string m_worldName;

		std::unordered_map<u16, Player &> m_players;
};

#endif // CLIENT_HPP_
