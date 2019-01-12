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
	m_chunk.generate();

	sf::Packet packet;
	packet << Network::Command::ChunkData;
	for (u16 x = 0 ; x < CHUNK_WIDTH ; ++x) {
		for (u16 y = 0 ; y < CHUNK_HEIGHT ; ++y) {
			for (u16 z = 0 ; z < CHUNK_DEPTH ; ++z) {
				packet << u16(m_chunk.data()[x][y][z]);
			}
		}
	}

	client.tcpSocket->send(packet);
}

