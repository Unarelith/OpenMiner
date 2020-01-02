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
#include "Server.hpp"
#include "ServerPlayer.hpp"
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

	// FIXME: Duplicated with ClientWorld
	for(s32 z = 0 ; z < m_depth ; ++z) {
		for(s32 y = 0 ; y < m_height ; ++y) {
			for(s32 x = 0 ; x < m_width ; ++x) {
				s32 cx = x - m_width / 2;
				s32 cy = y - m_height / 2;
				s32 cz = z - m_depth / 2;

				Chunk *chunk = getChunk(cx, cy, cz);
				if(cx > -m_width / 2)      chunk->setSurroundingChunk(Chunk::Left,   getChunk(cx - 1, cy, cz));
				if(cx <  m_width / 2 - 1)  chunk->setSurroundingChunk(Chunk::Right,  getChunk(cx + 1, cy, cz));
				if(cy > -m_height / 2)     chunk->setSurroundingChunk(Chunk::Bottom, getChunk(cx, cy - 1, cz));
				if(cy <  m_height / 2 - 1) chunk->setSurroundingChunk(Chunk::Top,    getChunk(cx, cy + 1, cz));
				if(cz > -m_depth / 2)      chunk->setSurroundingChunk(Chunk::Front,  getChunk(cx, cy, cz - 1));
				if(cz <  m_depth / 2 - 1)  chunk->setSurroundingChunk(Chunk::Back,   getChunk(cx, cy, cz + 1));
			}
		}
	}
}

void ServerWorld::update(Server &server, std::unordered_map<u16, ServerPlayer> &players) {
	for (auto &it : m_chunks) {
		it->tick(players, *this, server);
		it->update();

		if (it->isGenerated() && !it->isSent()) {
			for (auto &client : server.info().clients())
				sendChunkData(client, it.get());
			// DEBUG("Chunk updated at", it->x(), it->y(), it->z());
			it->setSent(true);
		}
	}
}

void ServerWorld::sendWorldData(Client &client) {
	// DEBUG("Sending world data...")
	for(s32 z = -m_depth / 8 ; z < m_depth / 8 ; z++) {
		for(s32 y = -m_height / 8 ; y < m_height / 8 ; y++) {
			for(s32 x = -m_width / 8 ; x < m_width / 8 ; x++) {
				sendChunkData(client, getChunk(x, y, z));
			}
		}
	}
	// DEBUG("Done sending world data")
}

void ServerWorld::sendChunkData(Client &client, ServerChunk *chunk) {
	chunk->generate();
	chunk->update();
	chunk->setInitialized(true);

	sf::Packet packet;
	packet << Network::Command::ChunkData;
	packet << chunk->x() << chunk->y() << chunk->z();
	for (u16 z = 0 ; z < CHUNK_DEPTH ; ++z) {
		for (u16 y = 0 ; y < CHUNK_HEIGHT ; ++y) {
			for (u16 x = 0 ; x < CHUNK_WIDTH ; ++x) {
				packet << u16(chunk->data()[x][y][z]);
				packet << chunk->lightmap().getLightData(x, y, z);
			}
		}
	}

	client.tcpSocket->send(packet);
	chunk->setSent(true);

	// std::cout << "Chunk at (" << chunk->x() << ", " << chunk->y() << ", " << chunk->z() << ") sent to client" << std::endl;
}

void ServerWorld::sendRequestedData(Client &client, int cx, int cy, int cz) {
	ServerChunk *chunk = getChunk(cx, cy, cz);
	if (chunk) {
		sendChunkData(client, chunk);
	}
}

ServerChunk *ServerWorld::getChunk(int cx, int cy, int cz) const {
	cx += m_width / 2;
	cy += m_height / 2;
	cz += m_depth / 2;

	if (cx < 0 || cx >= m_width || cy < 0 || cy >= m_height || cz < 0 || cz >= m_depth)
		return nullptr;

	return m_chunks.at(cx + cy * m_width + cz * m_width * m_height).get();
}

BlockData *ServerWorld::getBlockData(int x, int y, int z) const {
	int cx = (x + CHUNK_WIDTH * (m_width / 2)) / CHUNK_WIDTH;
	int cy = (y + CHUNK_HEIGHT * (m_height / 2)) / CHUNK_HEIGHT;
	int cz = (z + CHUNK_DEPTH * (m_depth / 2)) / CHUNK_DEPTH;

	if (cx < 0 || cx >= m_width || cy < 0 || cy >= m_height || cz < 0 || cz >= m_depth)
		return 0;

	Chunk *chunk = m_chunks.at(cx + cy * m_width + cz * m_width * m_height).get();
	if (chunk)
		return chunk->getBlockData(x & (CHUNK_WIDTH - 1), y & (CHUNK_HEIGHT - 1), z & (CHUNK_DEPTH - 1));
	return 0;
}

u16 ServerWorld::getBlock(int x, int y, int z) const {
	int cx = (x + CHUNK_WIDTH * (m_width / 2)) / CHUNK_WIDTH;
	int cy = (y + CHUNK_HEIGHT * (m_height / 2)) / CHUNK_HEIGHT;
	int cz = (z + CHUNK_DEPTH * (m_depth / 2)) / CHUNK_DEPTH;

	if (cx < 0 || cx >= m_width || cy < 0 || cy >= m_height || cz < 0 || cz >= m_depth)
		return 0;

	ServerChunk *chunk = m_chunks.at(cx + cy * m_width + cz * m_width * m_height).get();
	if (chunk)
		return chunk->getBlock(x & (CHUNK_WIDTH - 1), y & (CHUNK_HEIGHT - 1), z & (CHUNK_DEPTH - 1));
	return 0;
}

void ServerWorld::setBlock(int x, int y, int z, u16 id) {
	int cx = (x + CHUNK_WIDTH * (m_width / 2)) / CHUNK_WIDTH;
	int cy = (y + CHUNK_HEIGHT * (m_height / 2)) / CHUNK_HEIGHT;
	int cz = (z + CHUNK_DEPTH * (m_depth / 2)) / CHUNK_DEPTH;

	if (cx < 0 || cx >= m_width || cy < 0 || cy >= m_height || cz < 0 || cz >= m_depth)
		return;

	ServerChunk *chunk = m_chunks.at(cx + cy * m_width + cz * m_width * m_height).get();
	if (chunk)
		chunk->setBlock(x & (CHUNK_WIDTH - 1), y & (CHUNK_HEIGHT - 1), z & (CHUNK_DEPTH - 1), id);
}

u16 ServerWorld::getData(int x, int y, int z) const {
	int cx = (x + CHUNK_WIDTH * (m_width / 2)) / CHUNK_WIDTH;
	int cy = (y + CHUNK_HEIGHT * (m_height / 2)) / CHUNK_HEIGHT;
	int cz = (z + CHUNK_DEPTH * (m_depth / 2)) / CHUNK_DEPTH;

	if (cx < 0 || cx >= m_width || cy < 0 || cy >= m_height || cz < 0 || cz >= m_depth)
		return 0;

	ServerChunk *chunk = m_chunks.at(cx + cy * m_width + cz * m_width * m_height).get();
	if (chunk)
		return chunk->getData(x & (CHUNK_WIDTH - 1), y & (CHUNK_HEIGHT - 1), z & (CHUNK_DEPTH - 1));
	return 0;
}

void ServerWorld::setData(int x, int y, int z, u16 id) {
	int cx = (x + CHUNK_WIDTH * (m_width / 2)) / CHUNK_WIDTH;
	int cy = (y + CHUNK_HEIGHT * (m_height / 2)) / CHUNK_HEIGHT;
	int cz = (z + CHUNK_DEPTH * (m_depth / 2)) / CHUNK_DEPTH;

	if (cx < 0 || cx >= m_width || cy < 0 || cy >= m_height || cz < 0 || cz >= m_depth)
		return;

	ServerChunk *chunk = m_chunks.at(cx + cy * m_width + cz * m_width * m_height).get();
	if (chunk)
		chunk->setData(x & (CHUNK_WIDTH - 1), y & (CHUNK_HEIGHT - 1), z & (CHUNK_DEPTH - 1), id);
}

