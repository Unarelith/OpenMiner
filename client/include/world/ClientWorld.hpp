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

#include "ClientChunk.hpp"
#include "Network.hpp"

class Player;

class ClientWorld : public gk::IDrawable {
	public:
		ClientWorld();

		void update(Player &player);

		void receiveChunkData(sf::Packet &packet);

	private:
		void draw(gk::RenderTarget &target, gk::RenderStates states) const override;

		ClientChunk m_chunk;
};

#endif // CLIENTWORLD_HPP_
