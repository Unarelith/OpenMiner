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
#ifndef CLIENTINFO_HPP_
#define CLIENTINFO_HPP_

#include <memory>

#include <SFML/Network.hpp>

#include "server/core/NetworkInputHandler.hpp"

class ClientInfo {
	public:
		ClientInfo(u16 _id, const std::shared_ptr<sf::TcpSocket> &socket)
			: id(_id), tcpSocket(socket) {}

		u16 id;
		bool isReady = false;

		u32 previousKeyTimestamp = 0;

		std::shared_ptr<sf::TcpSocket> tcpSocket;

		NetworkInputHandler inputHandler;

		std::string playerName;

		u16 screenWidth = 1;
		u16 screenHeight = 1;
		u8 guiScale = 1;
};

#endif // CLIENTINFO_HPP_
