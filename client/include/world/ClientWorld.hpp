/*
 * =====================================================================================
 *
 *       Filename:  ClientWorld.hpp
 *
 *    Description:
 *
 *        Created:  12/01/2019 14:00:57
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef CLIENTWORLD_HPP_
#define CLIENTWORLD_HPP_

#include <unordered_map>

#include "ClientChunk.hpp"
#include "Network.hpp"
#include "World.hpp"

class ClientCommandHandler;
class TextureAtlas;

class ClientWorld : public World, public gk::Drawable {
	using ChunkMap = std::unordered_map<gk::Vector3i, std::unique_ptr<ClientChunk>>;

	public:
		ClientWorld();

		void init(float playerX, float playerY, float playerZ);
		void update();
		void sendChunkRequests();

		void receiveChunkData(sf::Packet &packet);
		void removeChunk(ChunkMap::iterator &it);

		Chunk *getChunk(int cx, int cy, int cz) const override;

		void setClient(ClientCommandHandler &client) { m_client = &client; }

		std::size_t loadedChunkCount() const { return m_chunks.size(); }

	private:
		void createChunkNeighbours(ClientChunk *chunk);

		void draw(gk::RenderTarget &target, gk::RenderStates states) const override;

		ChunkMap m_chunks;

		TextureAtlas &m_textureAtlas;

		ClientCommandHandler *m_client = nullptr;

		mutable float m_ud = 1000;
		mutable s32 m_ux;
		mutable s32 m_uy;
		mutable s32 m_uz;
};

#endif // CLIENTWORLD_HPP_
