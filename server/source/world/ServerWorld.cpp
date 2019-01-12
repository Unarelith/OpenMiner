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

ServerWorld::ServerWorld() {
	for(s32 z = 0 ; z < m_depth ; z++) {
		for(s32 y = 0 ; y < m_height ; y++) {
			for(s32 x = 0 ; x < m_width ; x++) {
				m_chunks.emplace_back(new ServerChunk(x - m_width / 2,
				                                      y - m_height / 2,
				                                      z - m_depth / 2));
			}
		}
	}

	for(s32 z = -m_depth / 2 ; z < m_depth / 2 ; z++) {
		for(s32 y = -m_height / 2 ; y < m_height / 2 ; y++) {
			for(s32 x = -m_width / 2 ; x < m_width / 2 ; x++) {
				ServerChunk *chunk = getChunk(x, y, z);
				if(x > -m_width / 2)      chunk->setSurroundingChunk(ServerChunk::Left,   getChunk(x - 1, y, z));
				if(x <  m_width / 2 - 1)  chunk->setSurroundingChunk(ServerChunk::Right,  getChunk(x + 1, y, z));
				if(y > -m_height / 2)     chunk->setSurroundingChunk(ServerChunk::Bottom, getChunk(x, y - 1, z));
				if(y <  m_height / 2 - 1) chunk->setSurroundingChunk(ServerChunk::Top,    getChunk(x, y + 1, z));
				if(z > -m_depth / 2)      chunk->setSurroundingChunk(ServerChunk::Front,  getChunk(x, y, z - 1));
				if(z <  m_depth / 2 - 1)  chunk->setSurroundingChunk(ServerChunk::Back,   getChunk(x, y, z + 1));
			}
		}
	}
}

void ServerWorld::update() {
	for (auto &it : m_chunks)
		it->generate();
}

void ServerWorld::sendWorldData(Client &client) {
	for (auto &it : m_chunks)
		sendChunkData(client, it.get());
}

void ServerWorld::sendChunkData(Client &client, ServerChunk *chunk) {
	chunk->generate();

	sf::Packet packet;
	packet << Network::Command::ChunkData;
	packet << chunk->x();
	packet << chunk->y();
	packet << chunk->z();
	for (u16 x = 0 ; x < CHUNK_WIDTH ; ++x) {
		for (u16 y = 0 ; y < CHUNK_HEIGHT ; ++y) {
			for (u16 z = 0 ; z < CHUNK_DEPTH ; ++z) {
				packet << u16(chunk->data()[x][y][z]);
			}
		}
	}

	client.tcpSocket->send(packet);

	std::cout << "Chunk at (" << chunk->x() << "," << chunk->y() << ", " << chunk->z() << ") sent to client" << std::endl;
}

ServerChunk *ServerWorld::getChunk(int cx, int cy, int cz) const {
	cx += m_width / 2;
	cy += m_height / 2;
	cz += m_depth / 2;

	if (cx < 0 || cx >= m_width || cy < 0 || cy >= m_height || cz < 0 || cz >= m_depth)
		return nullptr;

	return m_chunks.at(cx + cy * m_width + cz * m_width * m_height).get();
}

