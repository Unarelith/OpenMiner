/*
 * =====================================================================================
 *
 *       Filename:  ServerChunk.hpp
 *
 *    Description:
 *
 *        Created:  12/01/2019 01:32:59
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef SERVERCHUNK_HPP_
#define SERVERCHUNK_HPP_

#include <gk/core/IntTypes.hpp>

#include "Config.hpp"
#include "Chunk.hpp"

class ServerPlayer;
class Server;
class World;

class ServerChunk : public Chunk {
	public:
		ServerChunk(s32 x, s32 y, s32 z) : Chunk(x, y, z) {}

		void updateLights();
		void tick(std::unordered_map<u16, ServerPlayer> &players, World &world, Server &server);

		bool isSent() const { return m_isSent; }
		void setSent(bool isSent) { m_isSent = isSent; }

	private:
		bool m_isSent = false;
};

#endif // SERVERCHUNK_HPP_
