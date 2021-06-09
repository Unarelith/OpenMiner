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
#ifndef DEBUGLIGHTMAPVIEWER_HPP_
#define DEBUGLIGHTMAPVIEWER_HPP_

#include <optional>

#include <gk/graphics/RectangleShape.hpp>

#include "EngineConfig.hpp"
#include "Text.hpp"

class ClientPlayer;
class ClientWorld;

class DebugLightmapViewer : public gk::Transformable, public gk::Drawable {
	public:
		DebugLightmapViewer(const ClientPlayer &player);

		void update(const ClientWorld &world);

		static constexpr s32 totalSize = CHUNK_WIDTH * 8;

	private:
		void draw(gk::RenderTarget &target, gk::RenderStates states) const override;

		const ClientPlayer &m_player;

		Text m_chunkLightmapValues[CHUNK_WIDTH][CHUNK_DEPTH];

		std::optional<gk::Vector3i> m_playerChunkPos;
		std::optional<u32> m_playerRelativeZ;

		gk::RectangleShape m_playerRect;
};

#endif // DEBUGLIGHTMAPVIEWER_HPP_
