/*
 * =====================================================================================
 *
 *       Filename:  ServerInfo.cpp
 *
 *    Description:
 *
 *        Created:  26/01/2018 22:41:02
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "ServerInfo.hpp"

Client &ServerInfo::addClient(sf::IpAddress address, u16 port, const std::shared_ptr<sf::TcpSocket> &socket) {
	m_clients.emplace_back(m_clients.size(), address, port, socket);
	return m_clients.back();
}

Client *ServerInfo::getClient(u16 id) {
	auto it = std::find_if(m_clients.begin(), m_clients.end(), [id] (Client &client) { return client.id == id; });
	if (it == m_clients.end())
		return nullptr;

	return &*it;
}

void ServerInfo::removeClient(u16 id) {
	auto it = std::find_if(m_clients.begin(), m_clients.end(), [id] (Client &client) { return client.id == id; });
	if (it != m_clients.end())
		m_clients.erase(it);
}

