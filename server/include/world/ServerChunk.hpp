/*
 * =====================================================================================
 *
 *  OpenMiner
 *  Copyright (C) 2018-2020 Unarelith, Quentin Bazin <openminer@unarelith.net>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
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

class ServerChunk : public Chunk {
	public:
		ServerChunk(s32 x, s32 y, s32 z, World &world) : Chunk(x, y, z, world) {}

		void updateLights();

		void onBlockPlaced(int x, int y, int z, const Block &block) const;
		void tick(std::unordered_map<u16, ServerPlayer> &players, World &world, Server &server);

		bool isSent() const { return m_isSent; }
		void setSent(bool isSent) { m_isSent = isSent; }

	private:
		bool m_isSent = false;
};

#endif // SERVERCHUNK_HPP_
