/*
 * =====================================================================================
 *
 *       Filename:  ServerInfo.hpp
 *
 *    Description:
 *
 *        Created:  22/01/2018 19:03:23
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
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

struct Client {
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
