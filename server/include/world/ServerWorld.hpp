/*
 * =====================================================================================
 *
 *       Filename:  ServerWorld.hpp
 *
 *    Description:
 *
 *        Created:  12/01/2019 14:02:07
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef SERVERWORLD_HPP_
#define SERVERWORLD_HPP_

#include "ServerChunk.hpp"

class Client;

class ServerWorld {
	public:
		void sendChunkData(Client &client);

	private:
		ServerChunk m_chunk{0, 0, 0};
};

#endif // SERVERWORLD_HPP_
