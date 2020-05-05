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
#include <gk/core/input/GamePad.hpp>
#include <gk/core/input/InputHandler.hpp>
#include <gk/core/GameClock.hpp>
#include <gk/core/Exception.hpp>

#include "Client.hpp"

void Client::connect(sf::IpAddress serverAddress, u16 serverPort) {
	m_serverAddress = serverAddress;
	m_serverPort = serverPort;

	m_tcpSocket.reset(new sf::TcpSocket);
	if (serverAddress.toInteger() == 0 || m_tcpSocket->connect(serverAddress, serverPort, sf::seconds(5)) != sf::Socket::Done)
		throw ClientConnectException("Network error: Unable to connect to server " + serverAddress.toString() + ":" + std::to_string(serverPort));

	if (m_socket.bind(0) != sf::Socket::Done)
		throw ClientConnectException("Network error: Bind failed");

	sf::Packet packet;
	packet << Network::Command::ClientConnect << sf::IpAddress::getLocalAddress().toString() << m_socket.getLocalPort();
	m_tcpSocket->send(packet);

	sf::Packet answer;
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
	m_socket.setBlocking(false);

	m_isConnected = true;
}

void Client::disconnect() {
	sf::Packet packet;
	packet << Network::Command::ClientDisconnect;
	m_tcpSocket->send(packet);

	m_tcpSocket->disconnect();
}

void Client::send(sf::Packet &packet) {
	if (m_tcpSocket)
		m_tcpSocket->send(packet);
	else
		throw EXCEPTION("Network error: Trying to send a packet without being connected");
}

void Client::sendKeyState() {
	if (!m_keyUpdateTimer.isStarted())
		m_keyUpdateTimer.start();

	if (m_keyUpdateTimer.time() > 15) {
		gk::InputHandler *inputHandler = gk::GamePad::getInputHandler();
		if (inputHandler) {
			sf::Packet packet;
			packet << Network::Command::KeyState << gk::GameClock::getInstance().getTicks() << m_id;
			for (auto &it : inputHandler->keysPressed()) {
				packet << static_cast<u8>(it.first) << it.second;
			}

			m_socket.send(packet, m_serverAddress, m_serverPort);
		}

		m_keyUpdateTimer.reset();
		m_keyUpdateTimer.start();
	}
}

void Client::update() {
	sf::Packet packet;
	sf::IpAddress senderAddress;
	u16 senderPort;
	while (m_socket.receive(packet, senderAddress, senderPort) == sf::Socket::Done) {
		Network::Command command;
		packet >> command;

		// gkDebug() << "UDP Message of type" << Network::commandToString(command) << "received from:" << senderAddress << ":" << senderPort;
	}

	while (m_tcpSocket->receive(packet) == sf::Socket::Done) {
		Network::Command command;
		packet >> command;

		// gkDebug() << "TCP message received:" << Network::commandToString(command);

		auto it = m_commands.find(command);
		if (it != m_commands.end())
			it->second(packet);
	}
}

