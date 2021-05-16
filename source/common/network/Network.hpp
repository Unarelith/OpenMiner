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

		ServerTick            = 0x10,
		ServerClosed          = 0x11,

		ChunkData             = 0x20,
		ChunkUnload           = 0x21,

		PlayerPlaceBlock      = 0x30,
		PlayerDigBlock        = 0x31,
		PlayerInvUpdate       = 0x32,
		PlayerPosUpdate       = 0x33,
		PlayerRotUpdate       = 0x34,
		PlayerSpawn           = 0x35,
		PlayerChangeDimension = 0x36,
		PlayerHeldItemChanged = 0x37,
		PlayerReady           = 0x38,
		PlayerChunkPosUpdate  = 0x39,

		BlockUpdate           = 0x40,
		BlockActivated        = 0x41,
		BlockGUIData          = 0x42,
		BlockInvUpdate        = 0x43,
		BlockDataUpdate       = 0x44,

		EntitySpawn           = 0x50,
		EntityDespawn         = 0x51,
		EntityPosition        = 0x52,
		EntityRotation        = 0x53,
		EntityAnimation       = 0x54,
		EntityDrawableDef     = 0x55,

		ItemActivated         = 0x60,

		RegistryData          = 0x70,

		ChatMessage           = 0x80,

		KeyPressed            = 0x90,
	};

	std::string commandToString(Command command);
}

sf::Packet &operator<<(sf::Packet &packet, Network::Command command);
sf::Packet &operator>>(sf::Packet &packet, Network::Command &command);

#endif // NETWORK_HPP_
