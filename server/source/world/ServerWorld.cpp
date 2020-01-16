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

			it.second->updateLights();

			if (it.second->isGenerated() && !it.second->isSent()) {
				for (auto &client : server.info().clients())
					sendChunkData(client, it.second.get());
				// DEBUG("Chunk updated at", it->x(), it->y(), it->z());
				it.second->setSent(true);
			}
		}
	}
}

void ServerWorld::sendSpawnData(Client &client, ServerPlayer &player) {
	// Player chunk pos
	int pcx = std::floor(player.x() / CHUNK_WIDTH);
	int pcy = std::floor(player.y() / CHUNK_HEIGHT);
	int pcz = std::floor(player.z() / CHUNK_DEPTH);

	// Create a chunk at the current player position
	auto it = m_chunks.emplace(gk::Vector3i{pcx, pcy, pcz}, new ServerChunk(pcx, pcy, pcz));
	ServerChunk *chunk = it.first->second.get();

	// Load surrounding chunks, starting from the one we generated above
	std::queue<ServerChunk*> chunks;
	chunks.emplace(chunk);
	while (!chunks.empty()) {
		ServerChunk *chunk = chunks.front();
		chunks.pop();

		// Send the chunk to the client
		sendChunkData(client, chunk);

		DEBUG("Processing chunk in queue at", chunk->x(), chunk->y(), chunk->z());

		gk::Vector3i surroundingChunks[6] = {
			{chunk->x() - 1, chunk->y(),     chunk->z()},
			{chunk->x() + 1, chunk->y(),     chunk->z()},
			{chunk->x(),     chunk->y(),     chunk->z() - 1},
			{chunk->x(),     chunk->y(),     chunk->z() + 1},
			{chunk->x(),     chunk->y() - 1, chunk->z()},
			{chunk->x(),     chunk->y() + 1, chunk->z()},
		};

		for (u8 i = 0 ; i < 6 ; ++i) {
			// Check if this neighbour already exists, if yes then skip it
			Chunk *neighbour = getChunk(surroundingChunks[i].x, surroundingChunks[i].y, surroundingChunks[i].z);
			if (neighbour)
				continue;

			// Create our neighbour
			auto it = m_chunks.emplace(
				gk::Vector3i{
					surroundingChunks[i].x,
					surroundingChunks[i].y,
					surroundingChunks[i].z
				},
				new ServerChunk{
					surroundingChunks[i].x,
					surroundingChunks[i].y,
					surroundingChunks[i].z
				}
			);

			// Get the created neighbour
			neighbour = it.first->second.get();

			// Assign surrounding chunk pointers
			chunk->setSurroundingChunk(i, neighbour);
			neighbour->setSurroundingChunk((i % 2 == 0) ? i + 1 : i - 1, chunk);

			// Compute distance to player chunk
			int dx = std::abs(surroundingChunks[i].x - pcx);
			int dy = std::abs(surroundingChunks[i].y - pcy);
			int dz = std::abs(surroundingChunks[i].z - pcz);
			int distance = std::max(dx, std::max(dy, dz));

			// If the chunk is close enough, add it to the queue
			if (distance < Config::renderDistance)
				chunks.emplace((ServerChunk *)neighbour);
		}
	}
}

void ServerWorld::sendChunkData(Client &client, ServerChunk *chunk) {
	if (!chunk) return;

	chunk->generate();
	chunk->updateLights();

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

	Chunk *chunk = getChunk(cx, cy, cz);
	if (!chunk) {
		auto it = m_chunks.emplace(gk::Vector3i(cx, cy, cz), new ServerChunk(cx, cy, cz));
		chunk = it.first->second.get();
	}

	sendChunkData(client, (ServerChunk *)chunk);
}

Chunk *ServerWorld::getChunk(int cx, int cy, int cz) const {
	auto it = m_chunks.find({cx, cy, cz});
	if (it == m_chunks.end())
		return nullptr;

	return it->second.get();
}

