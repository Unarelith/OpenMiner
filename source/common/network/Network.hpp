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

namespace Network {
	using Packet = CompressedPacket;

	enum class Command {
		// Client commands
		ClientConnect         = 0x00,  // <TCP> [NetworkCommand]                         (from Client only)
		ClientDisconnect      = 0x01,  // <TCP> [NetworkCommand]                         (from Client only)
		ClientOk              = 0x02,  // <TCP> [NetworkCommand][u16 client id][bool isSingleplayer] (from Server only)
		ClientRefused         = 0x03,  // <TCP> [NetworkCommand]                         (from Server only)

		// Server commands
		ServerClosed          = 0x04,  // <TCP> [NetworkCommand][string message] (from Server only)

		// Chunk commands
		ChunkData             = 0x05,   // <TCP> [NetworkCommand][s32 cx, cy, cz][u32...] (from Server only)
		ChunkRequest          = 0x06,   // <TCP> [NetworkCommand][s32 cx, cy, cz]         (from Client only)

		// Player commands
		PlayerPlaceBlock      = 0x07,  // <TCP> [NetworkCommand][s32 x, y, z][u32 block] (from Client only)
		PlayerDigBlock        = 0x08,  // <TCP> [NetworkCommand][s32 x, y, z]            (from Client only)
		PlayerInvUpdate       = 0x09,  // <TCP> [NetworkCommand][u16 client id][[std::string item][u16 amount][u8 x, y]...] (both) [FIXME]
		PlayerPosUpdate       = 0x0a, // <TCP> [NetworkCommand][u16 client id][s32 x, y, z][bool isTeleportation] (both) // FIXME
		PlayerSpawn           = 0x0b, // <TCP> [NetworkCommand][u16 client id][s32 x, y, z]            (from Server only)
		PlayerInventory       = 0x0c, // <TCP> [NetworkCommand][u16 screenWidth, screenHeight][u8 guiScale] (from Client only)
		PlayerCreativeWindow  = 0x0d, // <TCP> [NetworkCommand][u16 screenWidth, screenHeight][u8 guiScale] (from Client only)
		PlayerChangeDimension = 0x0e, // <TCP> [NetworkCommand][u16 client id][s32 x, y, z][u16 dimension] (from Server only)
		PlayerHeldItemChanged = 0x0f, // <TCP> [NetworkCommand][u8 hotbar slot][u16 item id (to check match with server)] (from Client only)

		// Block commands
		BlockUpdate           = 0x10, // <TCP> [NetworkCommand][s32 x, y, z][u32 block] (from Server only)
		BlockActivated        = 0x11, // <TCP> [NetworkCommand][s32 x, y, z][u16 screenWidth, screenHeight][u8 guiScale] (from Client only)
		BlockGUIData          = 0x12, // <TCP> [NetworkCommand][LuaGUIData data]        (from Server only)
		BlockInvUpdate        = 0x13, // <TCP> [NetworkCommand][s32 x, y, z][[std::string item][u16 amount][u8 x, y]...] (both) [FIXME]
		BlockDataUpdate       = 0x14, // <TCP> [NetworkCommand][s32 x, y, z][u64 data]  (both) [FIXME]

		// Registry commands
		RegistryData          = 0x15, // <TCP> [NetworkCommand][Block block]            (from Server only)

		// Chat commands
		ChatMessage           = 0x16, // <TCP> [NetworkCommand][u16 client id][std::string message] (both)

		// Entity commands
		EntitySpawn           = 0x17, // <TCP> [NetworkCommand][u32 entity id] (from Server only)
		EntityDespawn         = 0x18, // <TCP> [NetworkCommand][u32 entity id] (from Server only)
		EntityPosition        = 0x19, // <TCP> [NetworkCommand][u32 entity id][double x, double y, double z] (from Server only)
		EntityRotation        = 0x1a, // <TCP> [NetworkCommand][u32 entity id][float w, float x, float y, float z] (from Server only)
		EntityAnimation       = 0x1b, // <TCP> [NetworkCommand][u32 entity id][AnimationComponent anim] (from Server only)
		EntityDrawableDef     = 0x1c, // <TCP> [NetworkCommand][u32 entity id][DrawableDef def] (from Server only)
	};

	std::string commandToString(Command command);
}

sf::Packet &operator<<(sf::Packet &packet, Network::Command command);
sf::Packet &operator>>(sf::Packet &packet, Network::Command &command);

#endif // NETWORK_HPP_
