/*
 * =====================================================================================
 *
 *  OpenMiner
 *
 *  Copyright (C) 2018-2020 Unarelith, Quentin Bazin <openminer@unarelith.net>
 *  Copyright (C) 2019-2020 the OpenMiner contributors (see CONTRIBUTORS.md)
 *
 *  This file is part of OpenMiner.
 *
 *  OpenMiner is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  OpenMiner is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with OpenMiner; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *
 * =====================================================================================
 */
#ifndef CLIENTWORLD_HPP_
#define CLIENTWORLD_HPP_

#include <map>
#include <unordered_map>
#include <unordered_set>

#include <gk/core/Vector4.hpp>
#include <gk/core/EventHandler.hpp>
#include <gk/gl/Camera.hpp>

#include "ChunkMeshBuilder.hpp"
#include "ClientChunk.hpp"
#include "ClientScene.hpp"
#include "Network.hpp"
#include "World.hpp"

class ClientCommandHandler;
class Sky;
class TextureAtlas;

class ClientWorld : public World, public gk::Drawable {
	using ChunkMap = std::unordered_map<gk::Vector3i, std::unique_ptr<ClientChunk>>;

	public:
		ClientWorld();

		void update(bool allowWorldReload);
		void requestChunkMeshing();
		void checkPlayerChunk(double playerX, double playerY, double playerZ);

		void clear();

		void changeDimension(u16 dimensionID);

		void receiveChunkData(Network::Packet &packet);
		void removeChunk(const gk::Vector3i &chunkPos);

		Chunk *getChunk(int cx, int cy, int cz) const override;

		void buildChunk(Chunk &chunk) { m_chunkMeshBuilder.addMeshBuildingJob(chunk, m_textureAtlas); }

		const ClientScene &scene() const { return m_scene; }
		ClientScene &scene() { return m_scene; }

		const ClientCommandHandler &client() const { return *m_client; }

		void setClient(ClientCommandHandler &client) { m_client = &client; }
		void setCamera(gk::Camera &camera) { m_camera = &camera; m_scene.setCamera(camera); }
		void setEventHandler(gk::EventHandler &eventHandler) { m_eventHandler = &eventHandler; }

		std::size_t loadedChunkCount() const { return m_chunks.size(); }

		const Sky *sky() const { return m_sky; }

	private:
		void createChunkNeighbours(ClientChunk *chunk);

		void draw(gk::RenderTarget &target, gk::RenderStates states) const override;

		const Dimension *m_dimension = nullptr;

		ClientScene m_scene;

		ChunkMap m_chunks;

		TextureAtlas &m_textureAtlas;

		ClientCommandHandler *m_client = nullptr;
		gk::Camera *m_camera = nullptr;
		gk::EventHandler *m_eventHandler = nullptr;

		const Sky *m_sky = nullptr;

		mutable std::set<gk::Vector3i> m_chunksToRemove;
		mutable std::multimap<float, gk::Vector3i> m_chunksToMesh;

		ChunkMeshBuilder m_chunkMeshBuilder{*this};
};

#endif // CLIENTWORLD_HPP_
