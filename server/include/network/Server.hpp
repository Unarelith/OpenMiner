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

#include <functional>

#include <SFML/Network/SocketSelector.hpp>
#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/UdpSocket.hpp>

#include "Network.hpp"
#include "ServerInfo.hpp"

class Server {
	using ConnectionCallback = std::function<void(Client&)>;
	using CommandCallback = std::function<void(sf::Packet &packet)>;

	public:
		void init(u16 port = 4242);

		void handleKeyState();

		void handleGameEvents();

		void sendToAllClients(sf::Packet &packet);

		bool isRunning() const { return m_isRunning; }
		bool hasGameStarted() const { return m_hasGameStarted; }

		ServerInfo &info() { return m_info; }

		sf::UdpSocket &udpSocket() { return m_udpSocket; }

		void setRunning(bool isRunning) { m_isRunning = isRunning; }
		void setGameStarted(bool hasGameStarted) { m_hasGameStarted = hasGameStarted; }

		void setConnectionCallback(const ConnectionCallback &callback) { m_connectionCallback = callback; }
		void setCommandCallback(Network::Command command, const CommandCallback &callback) { m_commands[command] = callback; }

	private:
		void handleNewConnections();
		void handleClientMessages();

		bool m_isRunning = true;
		bool m_hasGameStarted = false;

		ServerInfo m_info;

		sf::UdpSocket m_udpSocket;

		sf::TcpListener m_tcpListener;
		sf::SocketSelector m_selector;

		ConnectionCallback m_connectionCallback;

		std::unordered_map<Network::Command, CommandCallback> m_commands;
};

#endif // SERVER_HPP_
