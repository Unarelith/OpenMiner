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
#ifndef NETWORK_HPP_
#define NETWORK_HPP_

#include "CompressedPacket.hpp"

// Please update 'docs/network-protocol.md' if you change this
namespace Network {
	using Packet = CompressedPacket;

	enum class Command {
		// Client commands
		ClientConnect         = 0x00,
		ClientDisconnect      = 0x01,
		ClientOk              = 0x02,
		ClientRefused         = 0x03,

		ServerClosed          = 0x04,

		ChunkData             = 0x05,
		ChunkRequest          = 0x06,

		PlayerPlaceBlock      = 0x07,
		PlayerDigBlock        = 0x08,
		PlayerInvUpdate       = 0x09,
		PlayerPosUpdate       = 0x0a,
		PlayerRotUpdate       = 0x0b,
		PlayerSpawn           = 0x0c,
		PlayerChangeDimension = 0x0d,
		PlayerHeldItemChanged = 0x0e,

		BlockUpdate           = 0x0f,
		BlockActivated        = 0x10,
		BlockGUIData          = 0x11,
		BlockInvUpdate        = 0x12,
		BlockDataUpdate       = 0x13,

		ItemActivated         = 0x14,

		RegistryData          = 0x15,

		ChatMessage           = 0x16,

		EntitySpawn           = 0x17,
		EntityDespawn         = 0x18,
		EntityPosition        = 0x19,
		EntityRotation        = 0x1a,
		EntityAnimation       = 0x1b,
		EntityDrawableDef     = 0x1c,

		KeyPressed            = 0x1d,
	};

	std::string commandToString(Command command);
}

sf::Packet &operator<<(sf::Packet &packet, Network::Command command);
sf::Packet &operator>>(sf::Packet &packet, Network::Command &command);

#endif // NETWORK_HPP_
