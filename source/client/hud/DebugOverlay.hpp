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
#ifndef DEBUGOVERLAY_HPP_
#define DEBUGOVERLAY_HPP_

#include "Text.hpp"

class ClientPlayer;
class ClientWorld;

class DebugOverlay : public gk::Transformable, public Drawable {
	public:
		DebugOverlay(const ClientPlayer &player, const ClientWorld &world);

		void update(bool printOpenGLInfo);

	private:
		void draw(gk::RenderTarget &target, RenderStates states) const override;

		const ClientPlayer &m_player;
		const ClientWorld &m_world;

		Text m_versionText;
		Text m_positionText;
};

#endif // DEBUGOVERLAY_HPP_
