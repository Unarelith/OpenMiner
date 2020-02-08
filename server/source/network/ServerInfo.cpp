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

