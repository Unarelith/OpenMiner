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

		ServerTick            = 0x04,
		ServerClosed          = 0x05,

		ChunkData             = 0x06,
		ChunkRequest          = 0x07,

		PlayerPlaceBlock      = 0x08,
		PlayerDigBlock        = 0x09,
		PlayerInvUpdate       = 0x0a,
		PlayerPosUpdate       = 0x0b,
		PlayerRotUpdate       = 0x0c,
		PlayerSpawn           = 0x0d,
		PlayerChangeDimension = 0x0e,
		PlayerHeldItemChanged = 0x0f,

		BlockUpdate           = 0x10,
		BlockActivated        = 0x11,
		BlockGUIData          = 0x12,
		BlockInvUpdate        = 0x13,
		BlockDataUpdate       = 0x14,

		ItemActivated         = 0x15,

		RegistryData          = 0x16,

		ChatMessage           = 0x17,

		EntitySpawn           = 0x18,
		EntityDespawn         = 0x19,
		EntityPosition        = 0x1a,
		EntityRotation        = 0x1b,
		EntityAnimation       = 0x1c,
		EntityDrawableDef     = 0x1d,

		KeyPressed            = 0x1e,
	};

	std::string commandToString(Command command);
}

sf::Packet &operator<<(sf::Packet &packet, Network::Command command);
sf::Packet &operator>>(sf::Packet &packet, Network::Command &command);

#endif // NETWORK_HPP_
