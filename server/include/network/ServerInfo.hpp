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
#ifndef SERVERINFO_HPP_
#define SERVERINFO_HPP_

#include <algorithm>
#include <memory>
#include <vector>

#include <SFML/Network.hpp>

#include <gk/core/Exception.hpp>

#include "NetworkInputHandler.hpp"

class Client {
	public:
		Client(u16 _id, sf::IpAddress _address, u16 _port, const std::shared_ptr<sf::TcpSocket> &socket)
			: id(_id), address(_address), port(_port), tcpSocket(socket) {}

		u16 id;
		bool isReady = false;

		sf::IpAddress address;
		u16 port;

		u32 previousKeyTimestamp = 0;

		std::shared_ptr<sf::TcpSocket> tcpSocket;

		NetworkInputHandler inputHandler;
};

class ServerInfo {
	public:
		Client &addClient(sf::IpAddress address, u16 port, const std::shared_ptr<sf::TcpSocket> &socket);
		Client *getClient(u16 id);
		void removeClient(u16 id);

		std::vector<Client> &clients() { return m_clients; }
		const std::vector<Client> &clients() const { return m_clients; }

	private:
		std::vector<Client> m_clients;
};

#endif // SERVERINFO_HPP_
