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

class ClientWorld : public World, public gk::Drawable {
	public:
		ClientWorld();

		void init(float playerX, float playerY, float playerZ);
		void update();
		void sendChunkRequests();

		void receiveChunkData(sf::Packet &packet);

		Chunk *getChunk(int cx, int cy, int cz) const override;

		void setClient(ClientCommandHandler &client) { m_client = &client; }

	private:
		void createChunkNeighbours(ClientChunk *chunk);

		void draw(gk::RenderTarget &target, gk::RenderStates states) const override;

		std::unordered_map<gk::Vector3i, std::unique_ptr<ClientChunk>> m_chunks;

		gk::Texture &m_texture;

		ClientCommandHandler *m_client = nullptr;

		mutable float m_ud = 1000;
		mutable s32 m_ux;
		mutable s32 m_uy;
		mutable s32 m_uz;
};

#endif // CLIENTWORLD_HPP_
