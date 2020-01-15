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
#include <gk/core/GameClock.hpp>

#include "Config.hpp"
#include "Network.hpp"
#include "Server.hpp"
#include "ServerPlayer.hpp"
#include "ServerWorld.hpp"

ServerWorld::ServerWorld() {
}

void ServerWorld::update(Server &server, std::unordered_map<u16, ServerPlayer> &players) {
	if (m_lastTick < gk::GameClock::getTicks() / 50) {
		m_lastTick = gk::GameClock::getTicks() / 50;

		for (auto &it : m_chunks) {
			it.second->tick(players, *this, server);

			it.second->update();

			if (it.second->isGenerated() && !it.second->isSent()) {
				for (auto &client : server.info().clients())
					sendChunkData(client, it.second.get());
				// DEBUG("Chunk updated at", it->x(), it->y(), it->z());
				it.second->setSent(true);
			}
		}
	}
}

void ServerWorld::sendChunkData(Client &client, ServerChunk *chunk) {
	if (!chunk) return;

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

	std::cout << "Chunk at (" << chunk->x() << ", " << chunk->y() << ", " << chunk->z() << ") sent to client" << std::endl;
}

void ServerWorld::sendRequestedData(Client &client, int cx, int cy, int cz) {
	std::cout << "Chunk at (" << cx << ", " << cy << ", " << cz << ") requested" << std::endl;

	ServerChunk *chunk = getChunk(cx, cy, cz);
	if (!chunk) {
		auto it = m_chunks.emplace(gk::Vector3i(cx, cy, cz), new ServerChunk(cx, cy, cz));
		chunk = it.first->second.get();
	}

	sendChunkData(client, chunk);
}

ServerChunk *ServerWorld::getChunk(int cx, int cy, int cz) const {
	auto it = m_chunks.find({cx, cy, cz});
	if (it == m_chunks.end())
		return nullptr;

	return it->second.get();
}

BlockData *ServerWorld::getBlockData(int x, int y, int z) const {
	Chunk *chunk = getChunk(x / CHUNK_WIDTH, y / CHUNK_HEIGHT, z / CHUNK_DEPTH);
	if (chunk)
		return chunk->getBlockData(x & (CHUNK_WIDTH - 1), y & (CHUNK_HEIGHT - 1), z & (CHUNK_DEPTH - 1));

	return nullptr;
}

u16 ServerWorld::getBlock(int x, int y, int z) const {
	Chunk *chunk = getChunk(x / CHUNK_WIDTH, y / CHUNK_HEIGHT, z / CHUNK_DEPTH);
	if (chunk)
		return chunk->getBlock(x & (CHUNK_WIDTH - 1), y & (CHUNK_HEIGHT - 1), z & (CHUNK_DEPTH - 1));

	return 0;
}

void ServerWorld::setBlock(int x, int y, int z, u16 id) const {
	Chunk *chunk = getChunk(x / CHUNK_WIDTH, y / CHUNK_HEIGHT, z / CHUNK_DEPTH);
	if (chunk)
		chunk->setBlock(x & (CHUNK_WIDTH - 1), y & (CHUNK_HEIGHT - 1), z & (CHUNK_DEPTH - 1), id);
}

u16 ServerWorld::getData(int x, int y, int z) const {
	Chunk *chunk = getChunk(x / CHUNK_WIDTH, y / CHUNK_HEIGHT, z / CHUNK_DEPTH);
	if (chunk)
		return chunk->getData(x & (CHUNK_WIDTH - 1), y & (CHUNK_HEIGHT - 1), z & (CHUNK_DEPTH - 1));

	return 0;
}

void ServerWorld::setData(int x, int y, int z, u16 id) const {
	Chunk *chunk = getChunk(x / CHUNK_WIDTH, y / CHUNK_HEIGHT, z / CHUNK_DEPTH);
	if (chunk)
		chunk->setBlock(x & (CHUNK_WIDTH - 1), y & (CHUNK_HEIGHT - 1), z & (CHUNK_DEPTH - 1), id);
}

