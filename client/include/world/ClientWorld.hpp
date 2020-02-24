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

		void update();
		void sendChunkRequests();
		void checkPlayerChunk(double playerX, double playerY, double playerZ);

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
