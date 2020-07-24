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
#ifndef BLOCKCURSOR_HPP_
#define BLOCKCURSOR_HPP_

#include <gk/core/SDLHeaders.hpp>

#include "ClientWorld.hpp"
#include "Inventory.hpp"
#include "BlockGeometry.hpp"

class ClientCommandHandler;
class ClientPlayer;
class Hotbar;

class BlockCursor : public gk::Drawable {
	public:
		BlockCursor(ClientPlayer &player, ClientWorld &world, ClientCommandHandler &client);

		void onEvent(const SDL_Event &event, const Hotbar &hotbar);

		void update(const Hotbar &hotbar);

		void resetDestroyAnimation() { m_animationStart = 0; }

		const BlockState *currentBlock() const { return m_currentBlock; }

	private:
		void updateVertexBuffer(const BlockState &blockState, const u8f orientation);
		void updateAnimationVertexBuffer(const BlockState &blockState, const u8f orientation, int animationPos = -1);
		void updateVBOCoords(Vertex vertices[BlockGeometry::nFaces][BlockGeometry::nVertsPerFace],
		                     const BlockState &blockState, float face, u8f orientation);

		void draw(gk::RenderTarget &target, gk::RenderStates states) const override;

		glm::ivec4 findSelectedBlock() const;

		ClientPlayer &m_player;
		ClientWorld &m_world;
		ClientCommandHandler &m_client;

		gk::VertexBuffer m_vbo;
		gk::VertexBuffer m_animationVBO;

		unsigned int m_animationStart = 0;
		glm::ivec4 m_selectedBlock{0, 0, 0, -1};
		const BlockState *m_currentBlock = nullptr;
		const ItemStack *m_currentTool = nullptr;

		gk::Texture *m_blockDestroyTexture = nullptr;
};

#endif // BLOCKCURSOR_HPP_
