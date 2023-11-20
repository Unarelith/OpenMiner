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
#include "common/core/Exception.hpp"
#include "client/network/Client.hpp"

void Client::connect(sf::IpAddress serverAddress, u16 serverPort, Player &player) {
	m_serverAddress = serverAddress;
	m_serverPort = serverPort;

	m_tcpSocket.reset(new sf::TcpSocket);
	if (serverAddress.toInteger() == 0 || m_tcpSocket->connect(serverAddress, serverPort, sf::seconds(5)) != sf::Socket::Done)
		throw ClientConnectException("Network error: Unable to connect to server " + serverAddress.toString() + ":" + std::to_string(serverPort));

	Network::Packet packet;
	packet << Network::Command::ClientConnect << player.name();
	m_tcpSocket->send(packet);

	Network::Packet answer;
	m_tcpSocket->receive(answer);

	Network::Command command;
	answer >> command;
	if (command == Network::Command::ClientRefused)
		throw ClientConnectException("Server error: Connection refused. Server probably reached max player amount.");

	bool isSingleplayer;
	if (command != Network::Command::ClientOk)
		throw ClientConnectException("Network error: Expected 'ClientOk' packet.");

	answer >> m_id >> isSingleplayer;
	if (m_isSingleplayer != isSingleplayer)
		throw ClientConnectException("Client error: The server is not valid");

	m_tcpSocket->setBlocking(false);

	m_isConnected = true;
}

void Client::disconnect() {
	Network::Packet packet;
	packet << Network::Command::ClientDisconnect;
	m_tcpSocket->send(packet);

	m_tcpSocket->disconnect();
}

void Client::send(Network::Packet &packet) {
	if (m_tcpSocket)
		m_tcpSocket->send(packet);
	else
		throw EXCEPTION("Network error: Trying to send a packet without being connected");
}

void Client::update() {
	Network::Packet packet;
	while (m_tcpSocket->receive(packet) == sf::Socket::Done) {
		Network::Command command;
		packet >> command;

		// logDebug() << "TCP message received:" << Network::commandToString(command);

		auto it = m_commands.find(command);
		if (it != m_commands.end())
			it->second(packet);
	}
}
