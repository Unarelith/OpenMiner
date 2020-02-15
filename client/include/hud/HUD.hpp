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
#ifndef HUD_HPP_
#define HUD_HPP_

#include <gk/gl/Shader.hpp>

#include "BlockCursor.hpp"
#include "BlockInfoWidget.hpp"
#include "Crosshair.hpp"
#include "DebugOverlay.hpp"
#include "Hotbar.hpp"

class HUD : public gk::Transformable, public gk::Drawable {
	public:
		HUD(ClientPlayer &player, ClientWorld &world, ClientCommandHandler &client);

		void setup();

		void onEvent(const SDL_Event &event);

		void update();

	private:
		void draw(gk::RenderTarget &target, gk::RenderStates states) const override;

		gk::Shader m_shader;
		glm::mat4 m_orthoMatrix;

		Hotbar m_hotbar;

		BlockCursor m_blockCursor;
		Crosshair m_crosshair;

		DebugOverlay m_debugOverlay;
		bool m_isDebugOverlayVisible = false;

		BlockInfoWidget m_blockInfoWidget;

		Text m_fpsText;
};

#endif // HUD_HPP_
