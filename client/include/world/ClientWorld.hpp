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

#include <memory>

#include "ClientChunk.hpp"
#include "Network.hpp"
#include "World.hpp"

class ClientCommandHandler;

class ClientWorld : public World, public gk::IDrawable {
	public:
		ClientWorld();

		void update();

		void receiveChunkData(sf::Packet &packet);

		// FIXME: Duplicated with ServerWorld
		ClientChunk *getChunk(int cx, int cy, int cz) const;
		BlockData *getBlockData(int x, int y, int z) const override;

		// FIXME: Duplicated with ServerWorld
		u16 getBlock(int x, int y, int z) const override;
		void setBlock(int x, int y, int z, u16 id) override;
		u16 getData(int x, int y, int z) const override;
		void setData(int x, int y, int z, u16 id) override;

		void setClient(ClientCommandHandler &client) { m_client = &client; }

	private:
		void draw(gk::RenderTarget &target, gk::RenderStates states) const override;

		std::vector<std::unique_ptr<ClientChunk>> m_chunks;

		gk::Texture &m_texture;

		ClientCommandHandler *m_client = nullptr;
};

#endif // CLIENTWORLD_HPP_
