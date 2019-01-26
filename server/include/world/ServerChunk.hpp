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
#include "TerrainGenerator.hpp"

class Player;
class Server;
class World;

class ServerChunk : public Chunk {
	public:
		ServerChunk(s32 x, s32 y, s32 z) : Chunk(x, y, z) {}

		void update();
		void generate();
		void tick(Player &player, World &world, Server &server);

		bool isGenerated() const { return m_isGenerated; }
		void setGenerated(bool isGenerated) { m_isGenerated = isGenerated; }

		bool isSent() const { return m_isSent; }
		void setSent(bool isSent) { m_isSent = isSent; }

	private:
		bool m_isSent = false;
		bool m_isGenerated = false;

		TerrainGenerator m_terrainGenerator;
};

#endif // SERVERCHUNK_HPP_
