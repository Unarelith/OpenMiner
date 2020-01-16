/*
 * =====================================================================================
 *
 *       Filename:  Network.cpp
 *
 *    Description:
 *
 *        Created:  22/01/2018 14:12:53
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include <map>

#include <gk/core/IntTypes.hpp>

#include "Network.hpp"

std::string Network::commandToString(Network::Command command) {
	std::map<Network::Command, std::string> commandNames = {
		{Network::Command::ClientConnect,    "ClientConnect"},
		{Network::Command::ClientDisconnect, "ClientDisconnect"},
		{Network::Command::ClientOk,         "ClientOk"},
		{Network::Command::ClientRefused,    "ClientRefused"},

		{Network::Command::KeyState,         "KeyState"},

		{Network::Command::ChunkData,        "ChunkData"},

		{Network::Command::PlayerPlaceBlock, "PlayerPlaceBlock"},
		{Network::Command::PlayerDigBlock,   "PlayerDigBlock"},
		{Network::Command::PlayerInvUpdate,  "PlayerInvUpdate"},
		{Network::Command::PlayerPosUpdate,  "PlayerPosUpdate"},
		{Network::Command::PlayerSpawn,      "PlayerSpawn"},

		{Network::Command::BlockUpdate,      "BlockUpdate"},
		{Network::Command::BlockActivated,   "BlockActivated"},
		{Network::Command::BlockGUIData,     "BlockGUIData"},
		{Network::Command::BlockInvUpdate,   "BlockInvUpdate"},
		{Network::Command::BlockDataUpdate,  "BlockDataUpdate"},

		{Network::Command::RegistryData,     "RegistryData"},
	};
	return commandNames[command];
}

sf::Packet &operator<<(sf::Packet &packet, Network::Command command) {
	return packet << static_cast<u16>(command);
}

sf::Packet &operator>>(sf::Packet &packet, Network::Command &command) {
	u16 tmp;
	packet >> tmp;
	command = static_cast<Network::Command>(tmp);
	return packet;
}

