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
#include "Server.hpp"

void Server::init(u16 port) {
	if (m_udpSocket.bind(port) != sf::Socket::Done)
		throw EXCEPTION("Network error: Bind failed");

	m_udpSocket.setBlocking(false);

	if (m_tcpListener.listen(port) != sf::Socket::Done)
		throw EXCEPTION("Network error: Listen failed");

	m_tcpListener.setBlocking(false);

	m_selector.add(m_tcpListener);
}

void Server::handleKeyState() {
	sf::Packet packet;
	sf::IpAddress senderAddress;
	u16 senderPort;
	while (m_udpSocket.receive(packet, senderAddress, senderPort) == sf::Socket::Status::Done) {
		Network::Command command;
		u32 timestamp;
		u16 clientId;
		packet >> command >> timestamp >> clientId;

		// std::cout << "UDP Message of type '" << Network::commandToString(command) << "' received from: " << senderAddress << ":" << senderPort << std::endl;

		if (command == Network::Command::KeyState) {
			ClientInfo *client = m_info.getClient(clientId);
			if (client && client->previousKeyTimestamp < timestamp) {
				client->previousKeyTimestamp = timestamp;

				while (!packet.endOfPacket()) {
					u8 key;
					bool isPressed;
					packet >> key >> isPressed;

					if (client->inputHandler.keysPressed().at(key) != isPressed)
						DEBUG((int)key, "changed state to", isPressed ? "true" : "false");

					client->inputHandler.setKeyPressed(key, isPressed);
				}
			}
		}
	}
}

void Server::handleGameEvents() {
	if (m_selector.wait(sf::milliseconds(10))) {
		if (m_selector.isReady(m_tcpListener)) {
			handleNewConnections();
		}
		else {
			handleClientMessages();
		}
	}
}

void Server::handleNewConnections() {
	std::shared_ptr<sf::TcpSocket> clientSocket = std::make_shared<sf::TcpSocket>();
	if (m_tcpListener.accept(*clientSocket) == sf::Socket::Done) {
		sf::Packet packet;
		clientSocket->receive(packet);

		Network::Command command;
		packet >> command;
		if (command != Network::Command::ClientConnect)
			throw EXCEPTION("Network error: Expected 'ClientConnect' packet.");

		if (m_info.clients().size() < 5) {
			std::string address;
			u16 port;
			packet >> address >> port;

			ClientInfo &client = m_info.addClient(address, port, clientSocket);
			m_selector.add(*client.tcpSocket);

			sf::Packet outPacket;
			outPacket << Network::Command::ClientOk << client.id;
			client.tcpSocket->send(outPacket);
			// client.tcpSocket->setBlocking(false);

			if (m_connectionCallback)
				m_connectionCallback(client);
		}
		else {
			sf::Packet outPacket;
			outPacket << Network::Command::ClientRefused;
			clientSocket->send(outPacket);
		}
	}
	else {
		std::cerr << "Warning: Connection accept failed." << std::endl;
	}
}

void Server::handleClientMessages() {
	for (size_t i = 0 ; i < m_info.clients().size() ; ++i) {
		ClientInfo &client = m_info.clients()[i];
		if (m_selector.isReady(*client.tcpSocket)) {
			sf::Packet packet;
			sf::Socket::Status status = client.tcpSocket->receive(packet);
			if (status == sf::Socket::Done) {
				Network::Command command;
				packet >> command;

				// DEBUG("TCP message received:", Network::commandToString(command));

				if (m_isRunning) {
					for (auto &it : m_commands) {
						if (command == it.first)
							it.second(client, packet);

						if (command == Network::Command::ClientDisconnect) {
							disconnectClient(client);
							--i;

							break;
						}
					}
				}
			}
			else if (status == sf::Socket::Disconnected) {
				disconnectClient(client);
				--i;
			}
		}
	}
}

void Server::disconnectClient(ClientInfo &client) {
	sf::Packet packet;
	packet << Network::Command::ClientDisconnect << client.id;
	sendToAllClients(packet);

	m_commands[Network::Command::ClientDisconnect](client, packet);

	m_selector.remove(*client.tcpSocket);
	m_info.removeClient(client.id);

	if (m_isSingleplayer && m_info.clients().size() == 0) {
		m_tcpListener.close();
		m_isRunning = false;
	}
}

void Server::sendToAllClients(sf::Packet &packet) {
	for (ClientInfo &client : m_info.clients()) {
		client.tcpSocket->send(packet);
	}
}

