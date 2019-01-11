/*
 * =====================================================================================
 *
 *       Filename:  Server.hpp
 *
 *    Description:
 *
 *        Created:  23/01/2018 14:47:14
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef SERVER_HPP_
#define SERVER_HPP_

#include <SFML/Network/SocketSelector.hpp>
#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/UdpSocket.hpp>

#include "ServerInfo.hpp"

class Server {
	public:
		void init(u16 port = 4242);

		void handleKeyState();

		void handleGameEvents();

		bool isRunning() const { return m_isRunning; }
		bool hasGameStarted() const { return m_hasGameStarted; }

		ServerInfo &info() { return m_info; }

		sf::UdpSocket &udpSocket() { return m_udpSocket; }

		void setRunning(bool isRunning) { m_isRunning = isRunning; }
		void setGameStarted(bool hasGameStarted) { m_hasGameStarted = hasGameStarted; }

	private:
		void handleNewConnections();
		void handleClientMessages();

		bool m_isRunning = true;
		bool m_hasGameStarted = false;

		ServerInfo m_info;

		sf::UdpSocket m_udpSocket;

		sf::TcpListener m_tcpListener;
		sf::SocketSelector m_selector;
};

#endif // SERVER_HPP_
