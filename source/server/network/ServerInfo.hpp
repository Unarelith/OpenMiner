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
#ifndef SERVERINFO_HPP_
#define SERVERINFO_HPP_

#include <algorithm>
#include <vector>

#include <gk/core/Exception.hpp>

#include "ClientInfo.hpp"

class ServerInfo {
	public:
		ClientInfo &addClient(sf::IpAddress address, const std::shared_ptr<sf::TcpSocket> &socket);
		ClientInfo *getClient(u16 id);
		void removeClient(u16 id);

		std::vector<ClientInfo> &clients() { return m_clients; }
		const std::vector<ClientInfo> &clients() const { return m_clients; }

	private:
		std::vector<ClientInfo> m_clients;
};

#endif // SERVERINFO_HPP_
