/*
 * =====================================================================================
 *
 *       Filename:  Network.hpp
 *
 *    Description:
 *
 *        Created:  22/01/2018 14:11:41
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef NETWORK_HPP_
#define NETWORK_HPP_

#include <SFML/Network/Packet.hpp>

namespace Network {
	enum class Command {
		// Client commands
		ClientConnect,     // <TCP> [NetworkCommand][u16 udp port]           (from Client only)
		ClientDisconnect,  // <TCP> [NetworkCommand]                         (from Client only)
		ClientOk,          // <TCP> [NetworkCommand][u16 client id]          (from Server only)
		ClientRefused,     // <TCP> [NetworkCommand]                         (from Server only)

		// Input commands
		KeyState,          // <UDP> [NetworkCommand][u32 timestamp][u16 client id][u32 keycode][bool isPressed]...

		// Chunk commands
		ChunkData,         // <TCP> [NetworkCommand][s32 cx, cy, cz][u32...] (from Server only)
		ChunkRequest,      // <TCP> [NetworkCommand][s32 cx, cy, cz]         (from Client only)

		// Player commands
		PlayerPlaceBlock,  // <TCP> [NetworkCommand][s32 x, y, z][u32 block] (from Client only)
		PlayerDigBlock,    // <TCP> [NetworkCommand][s32 x, y, z]            (from Client only)
		PlayerInvUpdate,   // <TCP> [NetworkCommand][u16 client id][[std::string item][u16 amount][u8 x, y]...] (both) [FIXME]
		PlayerPosUpdate,   // <TCP> [NetworkCommand][u16 client id][s32 x, y, z] (both) // FIXME
		PlayerSpawn,       // <TCP> [NetworkCommand][u16 client id][s32 x, y, z]            (from Server only)
		PlayerInventory,   // <TCP> [NetworkCommand]                         (from Client only)

		// Block commands
		BlockUpdate,       // <TCP> [NetworkCommand][s32 x, y, z][u32 block] (from Server only)
		BlockActivated,    // <TCP> [NetworkCommand][s32 x, y, z]            (from Client only)
		BlockGUIData,      // <TCP> [NetworkCommand][LuaGUIData data]        (from Server only)
		BlockInvUpdate,    // <TCP> [NetworkCommand][s32 x, y, z][[std::string item][u16 amount][u8 x, y]...] (both) [FIXME]
		BlockDataUpdate,   // <TCP> [NetworkCommand][s32 x, y, z][u64 data]  (both) [FIXME]

		// Registry commands
		RegistryData,      // <TCP> [NetworkCommand][Block block]            (from Server only)
	};

	std::string commandToString(Command command);
}

sf::Packet &operator<<(sf::Packet &packet, Network::Command command);
sf::Packet &operator>>(sf::Packet &packet, Network::Command &command);

#endif // NETWORK_HPP_
