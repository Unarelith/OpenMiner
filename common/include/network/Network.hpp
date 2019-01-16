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
		ClientConnect,     // <TCP> [NetworkCommand][u16 udp port]
		ClientOk,          // <TCP> [NetworkCommand][u16 client id]
		ClientRefused,     // <TCP> [NetworkCommand]
		ClientReady,       // <TCP> [NetworkCommand][u16 client id]
		ClientDisconnect,  // <TCP> [NetworkCommand]

		// Game commands
		GameStart,         // <TCP> [NetworkCommand]
		GameEnd,           // <TCP> [NetworkCommand]

		// Input commands
		KeyState,          // <UDP> [NetworkCommand][u32 timestamp][u16 client id][u32 keycode][bool isPressed]...

		// Chunk commands
		ChunkData,         // <TCP> [NetworkCommand][s32 sx, sy, sz][u32...] (from Server only)

		// Player commands
		PlayerPlaceBlock,  // <TCP> [NetworkCommand][s32 x, y, z][u32 block] (from Client only)
		PlayerDigBlock,    // <TCP> [NetworkCommand][s32 x, y, z]            (from Client only)

		// Block commands
		BlockUpdate,       // <TCP> [NetworkCommand][s32 x, y, z][u32 block] (from Server only)
	};

	std::string commandToString(Command command);
}

sf::Packet &operator<<(sf::Packet &packet, Network::Command command);
sf::Packet &operator>>(sf::Packet &packet, Network::Command &command);

#endif // NETWORK_HPP_
