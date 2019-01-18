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

class Client;

class ClientWorld : public gk::IDrawable {
	public:
		ClientWorld(Client &client);

		void update();

		void receiveChunkData(sf::Packet &packet);

		// FIXME: Duplicated with ServerWorld
		ClientChunk *getChunk(int cx, int cy, int cz) const;

		u16 getBlock(int x, int y, int z) const;
		void setBlock(int x, int y, int z, u16 id);

	private:
		void draw(gk::RenderTarget &target, gk::RenderStates states) const override;

		// FIXME: Duplicated with those in ServerWorld
		const s32 m_width = 32;
		const s32 m_height = 8;
		const s32 m_depth = 32;

		std::vector<std::unique_ptr<ClientChunk>> m_chunks;

		gk::Texture &m_texture;

		Client &m_client;
};

#endif // CLIENTWORLD_HPP_
