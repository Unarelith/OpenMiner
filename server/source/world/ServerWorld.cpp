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

		for (auto &it : players) {
			sendChunks(it.second);
		}

		for (auto &it : m_chunks) {
			it.second->tick(players, *this, server);

			if (it.second->isLightGenerated())
				it.second->updateLights();

			if (it.second->isGenerated() && !it.second->isSent()) {
				for (auto &client : server.info().clients())
					sendChunkData(client, it.second.get());

				DEBUG("Chunk updated at", it.second->x(), it.second->y(), it.second->z());
			}
		}
	}
}

void ServerWorld::sendChunks(const ServerPlayer &player) {
	// Player chunk pos
	int pcx = std::floor(player.x() / CHUNK_WIDTH);
	int pcy = std::floor(player.y() / CHUNK_HEIGHT);
	int pcz = std::floor(player.z() / CHUNK_DEPTH);

	std::queue<ServerChunk *> *chunkQueue;
	u8 *currentDistance;

	auto it = m_chunkQueues.find(player.clientID());
	if (it == m_chunkQueues.end()) {
		auto it = m_chunkQueues.emplace(player.clientID(), std::make_pair(std::queue<ServerChunk *>{}, 1));
		chunkQueue = &it.first->second.first;
		currentDistance = &it.first->second.second;
	}
	else {
		chunkQueue = &it->second.first;
		currentDistance = &it->second.second;
	}

	// Create a chunk at the current player position if it doesn't exist
	ServerChunk *chunk = (ServerChunk *)getChunk(pcx, pcy, pcz);
	if (!chunk) {
		auto it = m_chunks.emplace(gk::Vector3i{pcx, pcy, pcz}, new ServerChunk(pcx, pcy, pcz));
		chunk = it.first->second.get();

		// DEBUG("Creating chunk at", chunk->x(), chunk->y(), chunk->z());
	}

	// Add the chunk to the queue
	chunkQueue->emplace(chunk);

	// Load surrounding chunks, 4 at a time
	u8 chunksSent = 0;
	while (chunksSent < 4 && !chunkQueue->empty()) {
		ServerChunk *chunk = chunkQueue->front();
		chunkQueue->pop();

		// If the chunk is already generated, update lights and send it
		if (chunk->isGenerated()) {
			// DEBUG("Updating lights in chunk at", chunk->x(), chunk->y(), chunk->z());

			chunk->updateLights();
			chunk->setLightGenerated(true);

			sendChunkData(player.client(), chunk);
		}

		// DEBUG("Processing chunk in queue at", chunk->x(), chunk->y(), chunk->z());

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
			ServerChunk *neighbour = (ServerChunk *)getChunk(surroundingChunks[i].x, surroundingChunks[i].y, surroundingChunks[i].z);
			if (!neighbour) {
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
			}

			// DEBUG("Creating chunk at", neighbour->x(), neighbour->y(), neighbour->z());

			// Assign surrounding chunk pointers
			chunk->setSurroundingChunk(i, neighbour);
			neighbour->setSurroundingChunk((i % 2 == 0) ? i + 1 : i - 1, chunk);

			// Compute distance to player chunk
			int dx = std::abs(surroundingChunks[i].x - pcx);
			int dy = std::abs(surroundingChunks[i].y - pcy);
			int dz = std::abs(surroundingChunks[i].z - pcz);
			int distance = std::max(dx, std::max(dy, dz)); // FIXME

			// If the chunk is close enough, add it to the queue
			if (distance <= *currentDistance || (!chunk->isGenerated() && distance < Config::renderDistance))
				chunkQueue->emplace(neighbour);
			else {
				++(*currentDistance);
				if (*currentDistance >= Config::renderDistance)
					*currentDistance = 1;
			}
		}

		// DEBUG("Generating chunk at", chunk->x(), chunk->y(), chunk->z());

		// All neighbours are created, so generate the chunk, propagate the light and send it
		if (!chunk->isGenerated()) {
			chunk->generate();
			chunk->updateLights();
			sendChunkData(player.client(), chunk);

			chunkQueue->emplace(chunk);
		}

		++chunksSent;
	}
}

void ServerWorld::sendChunkData(const Client &client, ServerChunk *chunk) {
	sf::Packet packet;
	packet << Network::Command::ChunkData;
	packet << chunk->x() << chunk->y() << chunk->z();
	for (u16 x = 0 ; x < CHUNK_WIDTH ; ++x) {
		for (u16 y = 0 ; y < CHUNK_HEIGHT ; ++y) {
			for (u16 z = 0 ; z < CHUNK_DEPTH ; ++z) {
				packet << u16(chunk->data()[x][y][z]);
				packet << chunk->lightmap().getLightData(x, y, z);
			}
		}
	}

	client.tcpSocket->send(packet);
	chunk->setSent(true);

	// std::cout << "Chunk at (" << chunk->x() << ", " << chunk->y() << ", " << chunk->z() << ") sent to client" << std::endl;
}

Chunk *ServerWorld::getChunk(int cx, int cy, int cz) const {
	auto it = m_chunks.find({cx, cy, cz});
	if (it == m_chunks.end())
		return nullptr;

	return it->second.get();
}

