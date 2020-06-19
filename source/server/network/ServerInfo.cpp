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
#include "ServerInfo.hpp"

ClientInfo &ServerInfo::addClient(const std::shared_ptr<sf::TcpSocket> &socket) {
	m_clients.emplace_back(m_clients.size() + 1, socket);
	return m_clients.back();
}

ClientInfo *ServerInfo::getClient(u16 id) {
	auto it = std::find_if(m_clients.begin(), m_clients.end(), [id] (ClientInfo &client) { return client.id == id; });
	if (it == m_clients.end())
		return nullptr;

	return &*it;
}

void ServerInfo::removeClient(u16 id) {
	auto it = std::find_if(m_clients.begin(), m_clients.end(), [id] (ClientInfo &client) { return client.id == id; });
	if (it != m_clients.end())
		m_clients.erase(it);
}

