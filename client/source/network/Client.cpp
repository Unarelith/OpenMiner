/*
 * =====================================================================================
 *
 *       Filename:  Client.cpp
 *
 *    Description:
 *
 *        Created:  25/01/2018 12:26:43
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include <gk/core/input/GamePad.hpp>
#include <gk/core/input/InputHandler.hpp>
#include <gk/core/GameClock.hpp>
#include <gk/core/Exception.hpp>

#include "Client.hpp"
#include "Network.hpp"

void Client::connect(sf::IpAddress serverAddress, u16 serverPort) {
	m_serverAddress = serverAddress;
	m_serverPort = serverPort;

	m_tcpSocket.reset(new sf::TcpSocket);
	if (serverAddress.toInteger() == 0 || m_tcpSocket->connect(serverAddress, serverPort, sf::seconds(5)) != sf::Socket::Done)
		throw EXCEPTION("Network error: Unable to connect to server", serverAddress.toString() + ":" + std::to_string(serverPort));

	if (m_socket.bind(0) != sf::Socket::Done)
		throw EXCEPTION("Network error: Bind failed");

	sf::Packet packet;
	packet << Network::Command::ClientConnect << sf::IpAddress::getLocalAddress().toString() << m_socket.getLocalPort();
	m_tcpSocket->send(packet);

	sf::Packet answer;
	m_tcpSocket->receive(answer);

	Network::Command command;
	answer >> command;
	if (command == Network::Command::ClientRefused)
		throw EXCEPTION("Server error: You can't join an already started game.");

	if (command != Network::Command::ClientOk)
		throw EXCEPTION("Network error: Expected 'ClientOk' packet.");
	answer >> m_id;

	m_tcpSocket->setBlocking(false);
	m_socket.setBlocking(false);

	m_isConnected = true;
}

void Client::disconnect() {
	sf::Packet packet;
	packet << Network::Command::ClientDisconnect;
	m_tcpSocket->send(packet);
}

void Client::sendReady() {
	sf::Packet packet;
	packet << Network::Command::ClientReady << m_id;
	m_tcpSocket->send(packet);
}

void Client::sendKeyState() {
	if (!m_keyUpdateTimer.isStarted())
		m_keyUpdateTimer.start();

	if (m_keyUpdateTimer.time() > 15) {
		gk::InputHandler *inputHandler = gk::GamePad::getInputHandler();
		if (inputHandler) {
			sf::Packet packet;
			packet << Network::Command::KeyState << gk::GameClock::getTicks() << m_id;
			for (auto &it : inputHandler->keysPressed()) {
				packet << static_cast<u8>(it.first) << it.second;
			}

			m_socket.send(packet, m_serverAddress, m_serverPort);
		}

		m_keyUpdateTimer.reset();
		m_keyUpdateTimer.start();
	}
}

void Client::update(bool &hasGameStarted) {
	sf::Packet packet;
	sf::IpAddress senderAddress;
	u16 senderPort;
	while (m_socket.receive(packet, senderAddress, senderPort) == sf::Socket::Done) {
		Network::Command command;
		packet >> command;

		std::cout << "UDP Message of type '" << Network::commandToString(command) << "' received from: " << senderAddress << ":" << senderPort << std::endl;
	}

	while (m_tcpSocket->receive(packet) == sf::Socket::Done) {
		Network::Command command;
		packet >> command;

		if (command == Network::Command::GameStart) {
			hasGameStarted = true;
		}
		else if (command == Network::Command::GameEnd) {
			disconnect();
			break;
		}
	}
}

