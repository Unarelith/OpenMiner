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
#ifndef MINIMAP_HPP_
#define MINIMAP_HPP_

#include <gk/core/SDLHeaders.hpp>

#include "Events.hpp"
#include "RectangleShape.hpp"
#include "Text.hpp"

class ClientPlayer;

class Minimap : public Drawable, public gk::Transformable {
	public:
		Minimap();

		void update(const ClientPlayer &player, class ClientWorld &world);

		void onEvent(const SDL_Event &event);

		void onChunkCreatedEvent(const ChunkCreatedEvent &event);
		void onChunkRemovedEvent(const ChunkRemovedEvent &event);

		static const u16 chunkSize = 2;
		static const u16 minimapSize = 50;

	private:
		void updatePlayerFovVertexBuffer();

		void draw(RenderTarget &target, RenderStates states) const override;

		std::unordered_map<gk::Vector3i, std::pair<RectangleShape, Text>> m_chunks;

		RectangleShape m_border;

		gk::Vector3i m_playerChunkPos;
		RectangleShape m_playerChunk;

		VertexBuffer m_vbo;
		gk::Transform m_playerFovRotationTransform = gk::Transform::Identity;

		bool m_displayTimesReceived = false;
};

#endif // MINIMAP_HPP_
