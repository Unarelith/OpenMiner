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

#include <SFML/Network/Packet.hpp>

namespace Network {
	enum class Command {
		// Client commands
		ClientConnect         = 0,  // <TCP> [NetworkCommand][u16 udp port]           (from Client only)
		ClientDisconnect      = 1,  // <TCP> [NetworkCommand]                         (from Client only)
		ClientOk              = 2,  // <TCP> [NetworkCommand][u16 client id][bool isSingleplayer] (from Server only)
		ClientRefused         = 3,  // <TCP> [NetworkCommand]                         (from Server only)

		// Input commands
		KeyState              = 4,   // <UDP> [NetworkCommand][u32 timestamp][u16 client id][u32 keycode][bool isPressed]...

		// Chunk commands
		ChunkData             = 5,   // <TCP> [NetworkCommand][s32 cx, cy, cz][u32...] (from Server only)
		ChunkRequest          = 6,   // <TCP> [NetworkCommand][s32 cx, cy, cz]         (from Client only)

		// Player commands
		PlayerPlaceBlock      = 7,  // <TCP> [NetworkCommand][s32 x, y, z][u32 block] (from Client only)
		PlayerDigBlock        = 8,  // <TCP> [NetworkCommand][s32 x, y, z]            (from Client only)
		PlayerInvUpdate       = 9,  // <TCP> [NetworkCommand][u16 client id][[std::string item][u16 amount][u8 x, y]...] (both) [FIXME]
		PlayerPosUpdate       = 10, // <TCP> [NetworkCommand][u16 client id][s32 x, y, z][bool isTeleportation] (both) // FIXME
		PlayerSpawn           = 11, // <TCP> [NetworkCommand][u16 client id][s32 x, y, z]            (from Server only)
		PlayerInventory       = 12, // <TCP> [NetworkCommand][u16 screenWidth, screenHeight][u8 guiScale] (from Client only)
		PlayerCreativeWindow  = 13, // <TCP> [NetworkCommand][u16 screenWidth, screenHeight][u8 guiScale] (from Client only)
		PlayerChangeDimension = 14, // <TCP> [NetworkCommand][u16 client id][s32 x, y, z][u16 dimension] (from Server only)

		// Block commands
		BlockUpdate           = 15, // <TCP> [NetworkCommand][s32 x, y, z][u32 block] (from Server only)
		BlockActivated        = 16, // <TCP> [NetworkCommand][s32 x, y, z][u16 screenWidth, screenHeight][u8 guiScale] (from Client only)
		BlockGUIData          = 17, // <TCP> [NetworkCommand][LuaGUIData data]        (from Server only)
		BlockInvUpdate        = 18, // <TCP> [NetworkCommand][s32 x, y, z][[std::string item][u16 amount][u8 x, y]...] (both) [FIXME]
		BlockDataUpdate       = 19, // <TCP> [NetworkCommand][s32 x, y, z][u64 data]  (both) [FIXME]

		// Registry commands
		RegistryData          = 20, // <TCP> [NetworkCommand][Block block]            (from Server only)

		// Chat commands
		ChatMessage           = 21, // <TCP> [NetworkCommand][u16 client id][std::string message] (both)
	};

	std::string commandToString(Command command);
}

sf::Packet &operator<<(sf::Packet &packet, Network::Command command);
sf::Packet &operator>>(sf::Packet &packet, Network::Command &command);

#endif // NETWORK_HPP_
