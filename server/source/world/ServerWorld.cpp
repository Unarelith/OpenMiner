/*
 * =====================================================================================
 *
 *       Filename:  ServerWorld.cpp
 *
 *    Description:
 *
 *        Created:  12/01/2019 14:02:22
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "Config.hpp"
#include "Network.hpp"
#include "ServerInfo.hpp"
#include "ServerWorld.hpp"

void ServerWorld::sendChunkData(Client &client) {
	sf::Packet packet;
	packet << Network::Command::ChunkData;
	for (u64 i = 0 ; i < CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_DEPTH ; ++i)
		packet << u16(rand() % 2);

	client.tcpSocket->send(packet);
}

